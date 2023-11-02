
//GameCenter.mm

#import "GameCenter.h"
#import <GameKit/GameKit.h>
#import "cocos2d.h"
#import "AppController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "RootViewController.h"
#endif
//#import "cocos2d.h"
//#import "EAGLView.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

@implementation GameCenter
static UIViewController* currentModalViewController;
static NSMutableDictionary *achievementsDictionary;
+ (BOOL) startGameCenter
{
    if( [self isGameCenterAPIAvailable] == NO )
    {
        //        NSLog("startGameCenter Faile");
        return NO;
    }
    achievementsDictionary = [[NSMutableDictionary alloc] init];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error)
     {
         if (error == nil)
         {
             for (GKAchievement* achievement in achievements)
                 [achievementsDictionary setObject: achievement forKey: achievement.identifier];
         }
     }];
    
    [self authenticateLocalPlayer];
    
    //    NSLog("startGameCenter OK");
    return YES;
}

+ (BOOL) isGameCenterAPIAvailable
{
    // Check for pdresence of GKLocalPlayer class.
    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
    
    // The device must be running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (localPlayerClassAvailable && osVersionSupported);
}

+ (void) authenticateLocalPlayer
{
    GKLocalPlayer* localPlayer = [GKLocalPlayer localPlayer];
    [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
        if( localPlayer.isAuthenticated )
        {
            /*
             ///< NSString를 char*로 컨버팅을 해야...
             NSLog("Alias : %s", localPlayer.alias);
             NSLog("Player ID : %s", localPlayer.playerID);
             */
            NSLog(@"Alias : %@", localPlayer.alias);
            NSLog(@"Player ID : %@", localPlayer.playerID);
        }else{
            NSLog(@"GameCenter Authentication failed.: %@", error);
        }
    }];
}

+ (void) reportScore:(int64_t)score scoreType:(const char*)leaderboardID
{
    GKScore* scoreReporter = [[[GKScore alloc] initWithCategory:[NSString stringWithUTF8String: leaderboardID ]] autorelease];
    scoreReporter.value = score;
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if( error != nil )
        {
            //            CCMessageBox("reportScore Error", "Game Center");
            NSLog(@"report score error");
        }
    }];
}

UIViewController* tempUIView;

+ (void) showLeaderboard:(const char*)leaderboardID
{
    if (![self isGameCenterAPIAvailable]) {
        [self startGameCenter];
        return;
    }
    
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardController != nil) {
        leaderboardController.category = [NSString stringWithUTF8String:leaderboardID];
        leaderboardController.leaderboardDelegate = self;

        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        currentModalViewController = [AppController getInstance].viewController; //[[UIViewController alloc] init];
        [window addSubview:currentModalViewController.view];
        [currentModalViewController presentModalViewController:leaderboardController animated:YES];
    }
}
+(void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
//    UIWindow* window = [[UIApplication sharedApplication] keyWindow];
//    UIView* gameView = [[window subviews] objectAtIndex:0];
    [viewController dismissViewControllerAnimated:YES completion: ^(void) {
        //        [tempUIView.view removeFromSuperview];
        //        [tempUIView release];
        
        [currentModalViewController dismissModalViewControllerAnimated:YES];
        [currentModalViewController.view.superview removeFromSuperview];
//        [currentModalViewController release];
        
//        [window becomeFirstResponder];
//        [window bringSubviewToFront:gameView];
//        [window makeKeyAndVisible];
    } ];

//    [currentModalViewController dismissModalViewControllerAnimated:YES];
//    [currentModalViewController.view.superview removeFromSuperview];
//    [currentModalViewController release];
}
+ (void) showAchievements
{
    GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
    if (achievements != NULL)
    {
        achievements.achievementDelegate = self;
        //        achievements.gameCenterDelegate = self;
        //        achievements.viewState = GKGameCenterViewControllerStateAchievements;
        
//        tempUIView = [[UIViewController alloc] init];
//        [tempUIView presentModalViewController:achievements animated:YES];
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        currentModalViewController = [AppController getInstance].viewController; //[[UIViewController alloc] init];
        [window addSubview:currentModalViewController.view];
        [currentModalViewController presentModalViewController:achievements animated:YES];
    }
    
}
+ (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
{
    [viewController dismissViewControllerAnimated:YES completion: ^(void) {
        [currentModalViewController dismissModalViewControllerAnimated:YES];
        [currentModalViewController.view.superview removeFromSuperview];
    } ];
    
    /*UIWindow* window = [[UIApplication sharedApplication] keyWindow];
     UIView* gameView = [[window subviews] objectAtIndex:0];
     [viewController dismissViewControllerAnimated:YES completion: ^(void) {
     [tempUIView.view removeFromSuperview];
     [tempUIView release];
     
     [window becomeFirstResponder];
     [window bringSubviewToFront:gameView];
     [window makeKeyAndVisible];
     } ];
     
     [viewController dismissModalViewControllerAnimated:YES];
     [viewController.view.superview removeFromSuperview];
     [viewController release];*/
}

+ (void) sendAchievementComplete:(const char*)text percentage:(float) percent
{
    NSString* str = [NSString stringWithUTF8String:text];
    GKAchievement *achievement = [self getAchievementForIdentifier:str];
    NSLog(@"str: %@, per: %f", str, percent);
    if (achievement)
    {
        
        achievement.percentComplete = percent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error != nil)
             {
                 NSLog(@"Achievement submit error: %@", error);
                 // Log the error.
             }
         }];
    }
}

+ (GKAchievement*) getAchievementForIdentifier: (NSString*) identifier
{
    GKAchievement *achievement = [achievementsDictionary objectForKey:identifier];
    if (achievement == nil)
    {
        achievement = [[GKAchievement alloc] initWithIdentifier:identifier];
        [achievementsDictionary setObject:achievement forKey:achievement.identifier];
    }
    return achievement;
}
@end

#endif
//AppController.mm

/*
 
 //HelloWorldScene.mm   cpp를 mm으로 변경한다
 
 ///< 추가
 #include "GameCenter.h"
 
 void HelloWorld::menuCloseCallback(CCObject* pSender)
 {
 [GameCenter showLeaderboard];
 }*/