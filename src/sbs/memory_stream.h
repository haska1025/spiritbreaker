#ifndef _MEMORY_STREAM_H_
#define _MEMORY_STREAM_H_

#include <rtc_base/stream.h>

using namespace rtc;
/////////////////////////////////////////////////////////////////////////////
// MemoryStream is a simple implementation of a StreamInterface over in-memory
// data.  Data is read and written at the current seek position.  Reads return
// end-of-stream when they reach the end of data.  Writes actually extend the
// end of data mark.
///////////////////////////////////////////////////////////////////////////////

class MemoryStreamBase : public StreamInterface {
public:
    StreamState GetState() const override;
    StreamResult Read(void* buffer,
            size_t bytes,
            size_t* bytes_read,
            int* error) override;
    StreamResult Write(const void* buffer,
            size_t bytes,
            size_t* bytes_written,
            int* error) override;
    void Close() override;
    /*
    bool SetPosition(size_t position) override;
    bool GetPosition(size_t* position) const override;
    bool GetSize(size_t* size) const override;
    bool GetAvailable(size_t* size) const override;
    bool ReserveSize(size_t size) override;
    */

    char* GetBuffer() { return buffer_; }
    const char* GetBuffer() const { return buffer_; }

protected:
    MemoryStreamBase();

    virtual StreamResult DoReserve(size_t size, int* error);

    // Invariant: 0 <= seek_position <= data_length_ <= buffer_length_
    char* buffer_;
    size_t buffer_length_;
    size_t data_length_;
    size_t seek_position_;

private:
    RTC_DISALLOW_COPY_AND_ASSIGN(MemoryStreamBase);
};

// MemoryStream dynamically resizes to accomodate written data.
/*
class MemoryStream : public MemoryStreamBase {
public:
    MemoryStream();
    explicit MemoryStream(const char* data);  // Calls SetData(data, strlen(data))
    MemoryStream(const void* data, size_t length);  // Calls SetData(data, length)
    ~MemoryStream() override;

    void SetData(const void* data, size_t length);

protected:
    StreamResult DoReserve(size_t size, int* error) override;
    // Memory Streams are aligned for efficiency.
    static const int kAlignment = 16;
    char* buffer_alloc_;
};
*/
// ExternalMemoryStream adapts an external memory buffer, so writes which would
// extend past the end of the buffer will return end-of-stream.

class ExternalMemoryStream : public MemoryStreamBase {
public:
    ExternalMemoryStream();
    ExternalMemoryStream(void* data, size_t length);
    ~ExternalMemoryStream() override;

    void SetData(void* data, size_t length);
};


#endif//_MEMORY_STREAM_H_

