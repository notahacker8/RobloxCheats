

@interface RecognizerDelegate : NSObject <NSSpeechRecognizerDelegate>

@property (nonatomic) NSSpeechRecognizer *recognizer;
@property (nonatomic) BOOL didRecognize;
@property (nonatomic) NSString* spokenCommand;

@end

