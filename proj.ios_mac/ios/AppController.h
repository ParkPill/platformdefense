#import <UIKit/UIKit.h>
#import <UnityAds/UnityAds.h>


@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UnityAdsDelegate> {
    UIWindow *window;
    AppController* _appController;
    NSMutableDictionary* dicSaveData;
    UIAlertView* progressAlert;
}
-(void)saveGame;
-(void)loadGame;
-(void)setInt:(NSString*) key num:(int)num;
-(void)setString:(NSString*) key str:(NSString*)num;
-(void)showUnityAdsVideo;
+(AppController*)getInstance;
@property(nonatomic, readonly) RootViewController* viewController;

@end

