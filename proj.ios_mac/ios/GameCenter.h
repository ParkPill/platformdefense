#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>


@interface GameCenter : NSObject<GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate>
{
    
}
+ (BOOL) startGameCenter;
///< 게임센터 사용가능한가?
+ (BOOL) isGameCenterAPIAvailable;
///< 접속, 인증
+ (void) authenticateLocalPlayer;
///< 점수 요청
+ (void) reportScore:(int64_t)score scoreType:(const char*)leaderboardID;
+ (void) showLeaderboard:(const char*)leaderboardID;

+ (void) showAchievements;

+ (void) sendAchievementComplete:(const char*)text percentage:(float) percent;
+ (GKAchievement*) getAchievementForIdentifier: (NSString*) identifier;

//@property(nonatomic, retain) NSMutableDictionary *achievementsDictionary;
@end