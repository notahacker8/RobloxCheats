
#include "Recognition.h"

@implementation RecognizerDelegate

- (id)init
{
    if ((self = [super init])) {
        self.spokenCommand = @"";
        self.didRecognize = NO;
        self.recognizer = [[NSSpeechRecognizer alloc] init];
        self.recognizer.listensInForegroundOnly = NO;
        self.recognizer.blocksOtherRecognizers = YES;
        self.recognizer.delegate = self;
        [self.recognizer startListening];
    }

    return self;
}

- (void)speechRecognizer:(NSSpeechRecognizer *)sender didRecognizeCommand:(NSString *)command
{
    self.didRecognize = YES;
    self.spokenCommand = command;
}

@end
