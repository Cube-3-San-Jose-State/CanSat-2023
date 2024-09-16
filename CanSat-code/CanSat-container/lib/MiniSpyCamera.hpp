#pragma once
namespace CanSat
{
    class MiniSpyCamera
    {
        private:
           int trig;
        public:
            MiniSpyCamera(int trig_pin)
            {
               trig = trig_pin;
               pinMode(trig, OUTPUT);
            }
            

        void TakeImage(){
            digitalWrite(trig, LOW);   
            delay(50);               
            digitalWrite(trig, HIGH);   
            delay(1000); 
        }

        void TakeVideo(int video_length){
            digitalWrite(trig, HIGH);   
            delay(video_length);        
            digitalWrite(trig, LOW); 
            delay(1000);
        }


    };
}
