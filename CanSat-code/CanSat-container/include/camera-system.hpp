#pragma once
#include "..\lib\MiniSpyCamera.hpp"

namespace CanSat
{
    class CameraSystem
    {
    private:
        MiniSpyCamera camera1;
        MiniSpyCamera camera2;
        MiniSpyCamera camera3;
        MiniSpyCamera camera4;
        MiniSpyCamera camera5;

    public:
        CameraSystem(int pin1, int pin2, int pin3, int pin4, int pin5)
        : camera1(pin1), camera2(pin2), camera3(pin3), camera4(pin4), camera5(pin5)
        {
        }

        void TakeVideoAll(int video_length)
        {
            camera1.TakeVideo(video_length);
            camera2.TakeVideo(video_length);
            camera3.TakeVideo(video_length);
            camera4.TakeVideo(video_length);
            camera5.TakeVideo(video_length);
        }

        void TakeImageAll()
        {
            camera1.TakeImage();
            camera2.TakeImage();
            camera3.TakeImage();
            camera4.TakeImage();
            camera5.TakeImage();
        }
    };
}
