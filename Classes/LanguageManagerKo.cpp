#include "LanguageManager.h"

void LanguageManager::setKorean(){
    strArray.clear();
    strArray.push_back(std::string("BM-HANNA.ttf")); // 0
    strArray.push_back(std::string("새 게임?")); // 1
    strArray.push_back(std::string("화면을 누르고 있으면 냄비를 발사합니다. 적들을 처치하고 코인을 얻으세요!")); // 2
    strArray.push_back(std::string("오른쪽 트랩 버튼을 누르고 전투화면의 깜빡이는 사각형을 선택하면 트랩을 설치할 수 있습니다.")); // 3
    strArray.push_back(std::string("설치된 트랩을 선택하면 트랩을 업그레이드 할 수 있습니다.")); // 4
    strArray.push_back(std::string("당황하지 마세요! 게임에 실패해도 트랩은 사라지지 않습니다! 코인을 모아 트랩을 더 강력하게 만드세요!")); // 5
    strArray.push_back(std::string("체크포인트! 죽어도 여기서부터 다시 시작합니다 ^^")); // 6
    strArray.push_back(std::string("더 이상 알려드릴 게 없네요! 효율적인 트랩 설치는 고득점의 열쇠! Wave 100까지 도전!")); // 7
    strArray.push_back(std::string("노멀: 별 거 없음")); // 8
    strArray.push_back(std::string("슬림: 조금 빠름")); // 9
    strArray.push_back(std::string("하드: 스파이크 무시")); // 10
    strArray.push_back(std::string("헬시: 조금 터프함")); // 11
    strArray.push_back(std::string("헤비: 느리지만 견고함")); // 12
    strArray.push_back(std::string("플레임: 불에 타지 않음")); // 13
    strArray.push_back(std::string("라이트: 빠르지만 약함")); // 14
    strArray.push_back(std::string("아이스: 절대 얼지 않음!")); // 15
    strArray.push_back(std::string("스카이: 땅에 설치된 트랩 무시")); // 16
    strArray.push_back(std::string("보스: 겁나 터프함!")); // 17
    strArray.push_back(std::string("얼음은 불을 끕니다.")); // 18
    strArray.push_back(std::string("불은 얼음을 녹입니다.")); // 19
    strArray.push_back(std::string("비행형 적들은 지상 트랩의 공격을 받지 않습니다.")); // 20
    strArray.push_back(std::string("큰 손도 큰 몬스터는 밀어 떨어뜨릴 수 없습니다.")); // 21
    strArray.push_back(std::string("고속형 적들은 불과 가시로부터 더 적은 데미지를 받습니다.")); // 22
    strArray.push_back(std::string("처음 불이 붙을 때 2배의 데미지를 받습니다!")); // 23
    strArray.push_back(std::string("빠르거나 무거운 적들은 오래 얼리기 힘들어요.")); // 24
    strArray.push_back(std::string("지금까지 사람들이 얼마나 많은 적들을 처치했는지 랭킹에서 확인하세요!")); // 25
    strArray.push_back(std::string("효율적인 트랩 업그레이드로 높은 점수를 얻을 수 있습니다!")); // 26
    strArray.push_back(std::string("벽 트랩은 업그레이드시 강해질 뿐만 아니라 체력도 보충됩니다!")); // 27
    strArray.push_back(std::string("적 20마리를 처리할 때마다 1개의 별을 얻습니다.")); // 28
    strArray.push_back(std::string("빨리감기 모드에선 난이도가 상승합니다!")); // 29
    strArray.push_back(std::string("대박! 이건 공유해야 돼!")); // 30
    strArray.push_back(std::string("이걸 나만 알고 있으면 안돼! 올려야 해!")); // 31
    strArray.push_back(std::string("이건 진짜 포스팅감!")); // 32
    strArray.push_back(std::string("알려야 해! 페친들에게!")); // 33
    strArray.push_back(std::string("모두가 알아야 할 사실이군. 포스팅해야겠어.")); // 34
    strArray.push_back(std::string("이 게임은 공유해야죠? 그렇죠? +0+")); // 35
    strArray.push_back(std::string("100 웨이브 클리어! 점수는 등록되었니다! 이제 Wave 1,000에 있는 엔딩을 확인하러 가보실까요? +_+")); // 36
    strArray.push_back(std::string("출석 보너스!\n헉! 매일 출석하면\n점점 많은 코인이?!")); // 37
    strArray.push_back(std::string("가스렌지(불): 적에게 불 속성 공격")); // 38
    strArray.push_back(std::string("냉장고: 적들을 잠깐동안 멈춘다")); // 39
    strArray.push_back(std::string("벽: 잠시동안 몬스터들을 막는다")); // 40
    strArray.push_back(std::string("플라스크: 1회성 폭탄")); // 41
    strArray.push_back(std::string("스파이크볼: 적들을 멈추고 큰 데미지를 준다")); // 42
    strArray.push_back(std::string("폭탄: 시한 폭한(1회성)")); // 43
    strArray.push_back(std::string("파이어(불): 적에게 불 속성 공격")); // 44
    strArray.push_back(std::string("스파이크: 밟는 적에게 데미지를 준다")); // 45
    strArray.push_back(std::string("스파이크드랍: 천장에서 가시를 떨어뜨려 공격한다")); // 46
    strArray.push_back(std::string("아이스볼(얼음): 적들에게 아이스볼을 발사하여 움직임을 멈춘다")); // 47
    strArray.push_back(std::string("큰손: 적들을 한 칸 밑으로 떨어뜨린다")); // 48
    strArray.push_back(std::string("월스파이크: 공격속도가 느리지만 적에게 큰 데미지를 준다")); // 49
    strArray.push_back(std::string("대공포 선인장:  공중 유닛만 공격한다")); // 50
    strArray.push_back(std::string("Wave 50 이 후에\n 새 게임 시작 5번")); // 51
    strArray.push_back(std::string("대공포 없이\n Wave 70까지 도달")); // 52
    strArray.push_back(std::string("트랩 없이 Wave 50까지 도달")); // 53
    strArray.push_back(std::string("몬스터 100,000 마리 잡기")); // 54
    strArray.push_back(std::string("15일째 출석 보상 받기")); // 55
    strArray.push_back(std::string("코인은 저장되지 않습니다. 저장하시기 전에 모두 소진하시기 바랍니다.")); // 56
    strArray.push_back(std::string("이 게임은 어떠한 정보도 자동으로 서버에 저장하지 않습니다. 게임을 삭제하면 모든 데이터가 손실됩니다.")); // 57
    strArray.push_back(std::string("저장된 게임을 불러오면 모든 코인이 손실됩니다. 불러오시겠습니까?")); // 58
    strArray.push_back(std::string("재밌으신가요? 별점5개는 업데이트를 부릅니다! ^0^")); // 59
}