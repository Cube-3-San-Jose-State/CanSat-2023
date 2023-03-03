#pragma once
#include <SoftwareSerial.h>

namespace CanSat
{
    class XBEE
    {
    private:
        uint8_t RX_pin;
        uint8_t TX_pin;
        SoftwareSerial *XBEE_Serial;
        String message = "";

    public:
        XBEE(){}
        XBEE(int rx_pin_, int tx_pin_) : RX_pin(rx_pin_), TX_pin(tx_pin_)
        {
            XBEE_Serial = new SoftwareSerial(RX_pin, TX_pin);
        }

        void Initialize(int baudrate_=9600)
        {
            XBEE_Serial->begin(baudrate_);
        }

        String receiveData()
        {
            if (XBEE_Serial->available())
            {
                message = XBEE_Serial->readString();
            }
            return message;
        }

        void transmitData(String data)
        {
            XBEE_Serial->print(data);
        }
    };
}