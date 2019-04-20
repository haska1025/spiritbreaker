#include "publisher.h"


Publisher::Publisher(uint32_t id):id_(id)
{
}
Publisher::~Publisher()
{
}

bool Publisher::AddSubsciber(std::shared_ptr<Subscriber> s)
{
    return true;
}
bool Publisher::RemoveSubsciber(uint32_t id)
{
}
std::shared_ptr<Subscriber> Publisher::GetSubsciber(uint32_t id)
{
}

