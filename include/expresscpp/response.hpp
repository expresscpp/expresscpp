#pragma once

class Response
{
public:
    void SetStatus(uint8_t new_status) { status = new_status; }

private:
    uint8_t status;
};
