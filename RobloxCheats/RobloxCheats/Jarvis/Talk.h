
void jarvis_talk(char* str)
{
    NSString *string = [NSString stringWithUTF8String:str];
    AVSpeechUtterance *utterance = [[AVSpeechUtterance alloc] initWithString:string];
    utterance.voice = [AVSpeechSynthesisVoice voiceWithLanguage:@"en-GB"];
    utterance.pitchMultiplier = 1.0;
    utterance.preUtteranceDelay = 0.0;
    utterance.rate = 0.5;
    utterance.volume = 1;

    AVSpeechSynthesizer *synth = [[AVSpeechSynthesizer alloc] init];
    [synth speakUtterance:utterance];
    printf("%s\n", str);
}
