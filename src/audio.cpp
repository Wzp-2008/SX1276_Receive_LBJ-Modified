#include "audio.hpp"
bool beep_enabled = true;
bool sound_enabled = true;
uint8_t volume = 4; // 音量控制，范围 0-4，0 是静音，4 是最大
void playTone(uint16_t freq, uint16_t duration_ms)
{
    ledcChangeFrequency(PWM_CH, freq, PWM_RES);
    ledcWrite(PWM_CH, 128); // 50% 占空比
    delay(duration_ms);
    ledcWrite(PWM_CH, 0);
}
void playAudio(String name)
{
    Serial.println("Playing audio: " + name);

    File file = SD.open("/audios/" + name + ".wav");
    if (!file)
    {
        Serial.println("Failed to open audio file: " + name);
        return;
    }

    file.seek(dataStart);

    uint8_t sample;
    while (file.available())
    {
        if (file.read(&sample, 1) == 1)
        {
            int centered = (int)sample - 128;
            int scaled = centered * volume;
            int pwm = 128 + scaled;
            pwm = constrain(pwm, 0, 255);
            ledcWrite(PWM_CH, pwm);
            delayMicroseconds(125);
        }
    }
    file.close();
    Serial.println("Finished playing: " + name);
}

void sound_for_train(String type, String no, int8_t direction)
{
    type.replace(" ", "");
    type.replace("0", "");
    no.replace(" ", "");
    ledcChangeFrequency(PWM_CH, PWM_FREQ, PWM_RES);
    ledcWrite(PWM_CH, 0);
    ledcWrite(PWM_CH, 16);
    ledcWrite(PWM_CH, 32);
    ledcWrite(PWM_CH, 64);
    ledcWrite(PWM_CH, 128);
    if (type != "" && type != "NA")
    {
        playAudio(type);
    }
    for (char c : no)
    {
        playAudio(String(c));
    }
    playAudio("Train");
    if (direction == FUNCTION_UP)
        playAudio("up");
    else if (direction == FUNCTION_DOWN)
        playAudio("down");
    playAudio("though");
    ledcWrite(PWM_CH, 128);
    ledcWrite(PWM_CH, 64);
    ledcWrite(PWM_CH, 32);
    ledcWrite(PWM_CH, 16);
    ledcWrite(PWM_CH, 0);
}

void beep()
{
    if (!beep_enabled)
        return;

    for (byte i = 0; i < 3; i++)
    {
        playTone(880, 100);
        delay(50);
    }
}

void beep_warn()
{
    if (!beep_enabled)
        return;

    for (byte i = 0; i < 2; i++)
    {
        playTone(880, 100);
        delay(50);
        playTone(800, 100);
        delay(50);
    }
}

// 开机音效
void boot_sound()
{
    if (!beep_enabled)
        return;

    int notes[] = {262, 330, 392, 523}; // C4, E4, G4, C5
    int durations[] = {120, 120, 120, 220};

    for (int i = 0; i < 4; i++)
    {
        playTone(notes[i], durations[i]);
        delay(30);
    }
    // ledcChangeFrequency(PWM_CH, PWM_FREQ, PWM_RES);
    // sound_for_train("G", "1207", FUNCTION_UP);
}