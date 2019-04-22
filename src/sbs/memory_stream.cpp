#include "memory_stream.h"


#ifndef ALIGNP
#define ALIGNP(p, t)                                             \
      (reinterpret_cast<uint8_t*>(((reinterpret_cast<uintptr_t>(p) + \
                                      ((t) - 1)) & ~((t) - 1))))
#endif

/////////////////////////////////////////////////////////////////////////////
// MemoryStream
///////////////////////////////////////////////////////////////////////////////

MemoryStreamBase::MemoryStreamBase()
    : buffer_(nullptr), buffer_length_(0), data_length_(0), seek_position_(0) {}

    StreamState MemoryStreamBase::GetState() const {
        return SS_OPEN;
    }

StreamResult MemoryStreamBase::Read(void* buffer, size_t bytes,
        size_t* bytes_read, int* error) {
    if (seek_position_ >= data_length_) {
        return SR_EOS;
    }
    size_t available = data_length_ - seek_position_;
    if (bytes > available) {
        // Read partial buffer
        bytes = available;
    }
    memcpy(buffer, &buffer_[seek_position_], bytes);
    seek_position_ += bytes;
    if (bytes_read) {
        *bytes_read = bytes;
    }
    return SR_SUCCESS;
}

StreamResult MemoryStreamBase::Write(const void* buffer, size_t bytes,
        size_t* bytes_written, int* error) {
    size_t available = buffer_length_ - data_length_;
    if (0 == available) {
        // Increase buffer size to the larger of:
        // a) new position rounded up to next 256 bytes
        // b) double the previous length
        size_t new_buffer_length =
            std::max(((data_length_ + bytes) | 0xFF) + 1, buffer_length_ * 2);
        StreamResult result = DoReserve(new_buffer_length, error);
        if (SR_SUCCESS != result) {
            return result;
        }
        RTC_DCHECK(buffer_length_ >= new_buffer_length);
        available = buffer_length_ - data_length_;
    }

    if (bytes > available) {
        bytes = available;
    }
    memcpy(&buffer_[data_length_], buffer, bytes);
    data_length_ += bytes;
    /*
    if (data_length_ < seek_position_) {
        data_length_ = seek_position_;
    }
    */
    if (bytes_written) {
        *bytes_written = bytes;
    }
    return SR_SUCCESS;
}

void MemoryStreamBase::Close() {
    // nothing to do
}
/*
bool MemoryStreamBase::SetPosition(size_t position) {
    if (position > data_length_)
        return false;
    seek_position_ = position;
    return true;
}

bool MemoryStreamBase::GetPosition(size_t* position) const {
    if (position)
        *position = seek_position_;
    return true;
}

bool MemoryStreamBase::GetSize(size_t* size) const {
    if (size)
        *size = data_length_;
    return true;
}

bool MemoryStreamBase::ReserveSize(size_t size) {
    return (SR_SUCCESS == DoReserve(size, nullptr));
}
*/

bool MemoryStreamBase::GetAvailable(size_t* size) const {
    if (size)
        *size = data_length_ - seek_position_;
    return true;
}

StreamResult MemoryStreamBase::DoReserve(size_t size, int* error) {
    return (buffer_length_ >= size) ? SR_SUCCESS : SR_EOS;
}

///////////////////////////////////////////////////////////////////////////////
MemoryStream::MemoryStream() : buffer_alloc_(nullptr) {}

MemoryStream::MemoryStream(const char* data) : buffer_alloc_(nullptr) {
    SetData(data, strlen(data));
}

MemoryStream::MemoryStream(const void* data, size_t length)
    : buffer_alloc_(nullptr) {
        SetData(data, length);
    }

MemoryStream::~MemoryStream() {
    delete [] buffer_alloc_;
}

void MemoryStream::SetData(const void* data, size_t length) {
    data_length_ = buffer_length_ = length;
    delete [] buffer_alloc_;
    buffer_alloc_ = new char[buffer_length_ + kAlignment];
    buffer_ = reinterpret_cast<char*>(ALIGNP(buffer_alloc_, kAlignment));
    memcpy(buffer_, data, data_length_);
    seek_position_ = 0;
}

StreamResult MemoryStream::DoReserve(size_t size, int* error) {
    if (buffer_length_ >= size)
        return SR_SUCCESS;

    if (char* new_buffer_alloc = new char[size + kAlignment]) {
        char* new_buffer = reinterpret_cast<char*>(
                ALIGNP(new_buffer_alloc, kAlignment));
        memcpy(new_buffer, buffer_, data_length_);
        delete [] buffer_alloc_;
        buffer_alloc_ = new_buffer_alloc;
        buffer_ = new_buffer;
        buffer_length_ = size;
        return SR_SUCCESS;
    }

    if (error) {
        *error = ENOMEM;
    }
    return SR_ERROR;
}
///////////////////////////////////////////////////////////////////////////////

ExternalMemoryStream::ExternalMemoryStream() {
}

ExternalMemoryStream::ExternalMemoryStream(void* data, size_t length) {
    SetData(data, length);
}

ExternalMemoryStream::~ExternalMemoryStream() {
}

void ExternalMemoryStream::SetData(void* data, size_t length) {
    data_length_ = buffer_length_ = length;
    buffer_ = static_cast<char*>(data);
    seek_position_ = 0;
}
