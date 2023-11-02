/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "GameManager.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import "GameCenter.h"
@implementation AppController
#define STR_SAVE_DATA_NAME @"pd_saved_data"
#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;
static AppController *instance;
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:YES];
    [UIApplication sharedApplication].idleTimerDisabled = YES;

    // Use RootViewController manage CCEAGLView 
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    instance = self;
    
    dicSaveData = [[NSMutableDictionary alloc] init];

    // Initialize Unity Ads
//    [[UnityAds sharedInstance] setTestMode:YES];
//    [[UnityAds sharedInstance] setDebugMode:YES];
    [[UnityAds sharedInstance] setDelegate:self];
    
    [[UnityAds sharedInstance] startWithGameId:@"80616" andViewController:_viewController];
    
    [GameCenter startGameCenter];

    app->run();

    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                    didFinishLaunchingWithOptions:launchOptions];;
}

- (void)createProgressionAlertWithMessage
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    progressAlert = [[[UIAlertView alloc] initWithTitle:@"" message:@"Please wait..." delegate:self cancelButtonTitle:nil otherButtonTitles:nil] autorelease];
    
    [progressAlert show];
    
    UIActivityIndicatorView *indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    
    // Adjust the indicator so it is up a few pixels from the bottom of the alert
    indicator.center = CGPointMake(progressAlert.bounds.size.width / 2, progressAlert.bounds.size.height - 50);
    [indicator startAnimating];
    [progressAlert addSubview:indicator];
    [indicator release];
#endif
}

- (void)destroyProgressionAlert
{
    if(progressAlert != nil)
    {
        [progressAlert dismissWithClickedButtonIndex:0 animated:YES];
        progressAlert = nil;
    }
}

-(void)setInt:(NSString*) key num:(int)num{
    [dicSaveData setValue:[[NSNumber numberWithInt:num] stringValue] forKey:key];
}
-(void)setString:(NSString*) key str:(NSString*)str{
    [dicSaveData setValue:str forKey:key];
}

-(void)saveGame{
    if ([GameCenter isGameCenterAPIAvailable]) {
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        NSData *data = [NSKeyedArchiver archivedDataWithRootObject:dicSaveData];
        [self createProgressionAlertWithMessage];
        [localPlayer saveGameData:data withName:STR_SAVE_DATA_NAME completionHandler:(^(GKSavedGame *savedGame, NSError *error){
            if (!error) {
                [savedGame loadDataWithCompletionHandler:^(NSData *data, NSError *error) {
                    NSMutableDictionary *dic = (NSMutableDictionary*) [NSKeyedUnarchiver unarchiveObjectWithData:data];
                    NSLog(@"%@", dic);
                }];
            }else{
                NSLog(@"error: %@", error);
            }
            [self destroyProgressionAlert];
        })];
    }
}
-(void)loadGame{
    if ([GameCenter isGameCenterAPIAvailable]) {
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        [self createProgressionAlertWithMessage];
        [localPlayer fetchSavedGamesWithCompletionHandler:(^(NSArray *savedGames, NSError *error){
            if (!error) {
                for (GKSavedGame *savedGame in savedGames) {
                    if (![[savedGame name] isEqualToString:STR_SAVE_DATA_NAME]) {
                        NSLog(@"savedGame different: %@", [savedGame name]);
                        continue;
                    }
                    [savedGame loadDataWithCompletionHandler:^(NSData *data, NSError *error) {
                        NSMutableDictionary *dic = (NSMutableDictionary*) [NSKeyedUnarchiver unarchiveObjectWithData:data];
                        for (NSString* key in dic) {
                            id value = [dic objectForKey:key];
                            if ([value isKindOfClass:[NSString class]]) {
                                GameManager::getInstance()->setLoadedData([key UTF8String], [value UTF8String]);
                                NSLog(@"%@, %@", key, value);
                            }
                        }
                    }];
//                    [savedGame loadDataWithCompletionHandler:^(NSData *data, NSError *error) {}];
                }
            }else{
                NSLog(@"error: %@", error);
            }
            [self destroyProgressionAlert];
        })];
    }
}
- (void)unityAdsVideoCompleted:(NSString *)rewardItemKey skipped:(BOOL)skippe
{
    if(!skippe){
        GameManager::getInstance()->getHudLayer()->OnVideoEnded();
    }else{
        log("video skipped");
    }
}
+ (AppController*)getInstance
{
    return instance;
}
- (void)showUnityAdsVideo
{
    // Set the zone before checking readiness or attempting to show.
    [[UnityAds sharedInstance] setZone:@"rewardedVideoZone"];
    
    // Use the canShow method to check for zone readiness,
    //  then use the canShowAds method to check for ad readiness.
    if ([[UnityAds sharedInstance] canShow])
    {
        // If both are ready, show the ad.
        [[UnityAds sharedInstance] show];
    }
}

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                sourceApplication:sourceApplication
                                                       annotation:annotation];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    [FBSDKAppEvents activateApp];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
