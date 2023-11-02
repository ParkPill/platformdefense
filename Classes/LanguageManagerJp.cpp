#include "LanguageManager.h"

void LanguageManager::setJapanese(){
    strArray.clear();
    strArray.push_back(std::string("NotoSansCJKjp-Bold.otf")); // 0
    strArray.push_back(std::string("新しいゲーム?")); // 1
    strArray.push_back(std::string("画面を押していると、鍋が発射されます。敵を退けてコインをもらってください！")); // 2
    strArray.push_back(std::string("右のトラップボタンを押して、戦闘画面の点滅する四角を選択するとトラップが設置されます。")); // 3
    strArray.push_back(std::string("設置されたトラップを選択すると、トラップのアップグレードができます。")); // 4
    strArray.push_back(std::string("落ち着いてください！ゲームに失敗してもトラップは無くなりません。！コインを集めてトラップをより強化させてください！")); // 5
    strArray.push_back(std::string("チェックポイント！死亡してもここからスタートされます＾＾")); // 6
    strArray.push_back(std::string("もう教えることはありません！効率的なトラップ設置は高得点の鍵！ Wave 100まで挑戦！")); // 7
    strArray.push_back(std::string("ノーマル：何もない")); // 8
    strArray.push_back(std::string("スリム：少し早い")); // 9
    strArray.push_back(std::string("ハード：スパイク無視")); // 10
    strArray.push_back(std::string("ヘルシー：少しタフ")); // 11
    strArray.push_back(std::string("ヘビー：遅いが堅固")); // 12
    strArray.push_back(std::string("フレーム：火に焼けない")); // 13
    strArray.push_back(std::string("ライト：早いが弱い")); // 14
    strArray.push_back(std::string("アイス：絶対凍らない！")); // 15
    strArray.push_back(std::string("スカイ：地に設置されたトラップ")); // 16
    strArray.push_back(std::string("ボス：とてもタフ！")); // 17
    strArray.push_back(std::string("氷は火を消します。")); // 18
    strArray.push_back(std::string("火は氷を溶かします。")); // 19
    strArray.push_back(std::string("飛行型敵は地上トラップの攻撃を受けません。")); // 20
    strArray.push_back(std::string("大手も大物モンスターは落とせません。")); // 21
    strArray.push_back(std::string("高速型敵は火とトゲから受けるダメージが少なくなります。")); // 22
    strArray.push_back(std::string("最初火が付いた時はダメージが２倍になります。")); // 23
    strArray.push_back(std::string("早くて重い敵は凍らせるのが難しいです。")); // 24
    strArray.push_back(std::string("今までどの位敵を退けているか、ランキングで確認してください！")); // 25
    strArray.push_back(std::string("効率的なトラップアップグレードで高い点数が得られます！")); // 26
    strArray.push_back(std::string("壁トラップはアップグレード時、強化せれたり体力が補充されます！")); // 27
    strArray.push_back(std::string("敵20を退けるごとに1つの星が得られます。")); // 28
    strArray.push_back(std::string("早巻きモードでは難易度が上昇します！")); // 29
    strArray.push_back(std::string("すごい！これは共有しなきゃ！")); // 30
    strArray.push_back(std::string("これを私だけ知っているのはもったいない！知らせなきゃ！")); // 31
    strArray.push_back(std::string("本当にこれはポスティングしなきゃ！")); // 32
    strArray.push_back(std::string("知らせなきゃ！フェイスブック友たちに！")); // 33
    strArray.push_back(std::string("みんなが知るべき事実だ。ポスティングしなきゃ。")); // 34
    strArray.push_back(std::string("このゲームは共有しなきゃ？そうでしょう？")); // 35
    strArray.push_back(std::string("100ウェーブクリア！点数が登録されました！これから Wave1,000にあるエンディングを確認しましょうか？")); // 36
    strArray.push_back(std::string("出席ボーナス！!\nうわー!\n毎日出席すると!\nどんどんコインが貯まる？！")); // 37
    strArray.push_back(std::string("ガスレンジ（火）：敵に火の属性攻撃")); // 38
    strArray.push_back(std::string("冷蔵庫：敵をしばらく止まらせる。")); // 39
    strArray.push_back(std::string("壁：しばらくモンスターたちを防ぐ")); // 40
    strArray.push_back(std::string("フラスコ：1回限り爆弾")); // 41
    strArray.push_back(std::string("スパイクボール：敵を止めて大きいダメージを与える")); // 42
    strArray.push_back(std::string("爆弾：時限爆弾（1回限り）")); // 43
    strArray.push_back(std::string("ファイヤ（火）：敵に火属性攻撃")); // 44
    strArray.push_back(std::string("スパイク：踏むと敵にダメージを与える")); // 45
    strArray.push_back(std::string("スパイクドロップ：天井からトゲを落として攻撃する")); // 46
    strArray.push_back(std::string("アイスボール（氷）：敵にアイスボールを発射して動きを止める")); // 47
    strArray.push_back(std::string("大手：敵を一段落とす")); // 48
    strArray.push_back(std::string("ウォールスパイク：攻撃速度が遅いが、敵に大きいダメージを与える")); // 49
    strArray.push_back(std::string("対空砲サボテン：空中ユニットだけ攻撃する")); // 50
    strArray.push_back(std::string("Wave 50以降に\n新しいゲームスタート5番")); // 51
    strArray.push_back(std::string("対空砲なしに\nWave70まで到達")); // 52
    strArray.push_back(std::string("トラップなしにWave50まで到達")); // 53
    strArray.push_back(std::string("モンスター100,000匹捕まえる")); // 54
    strArray.push_back(std::string("15日目、出席補償をもらる")); // 55
    strArray.push_back(std::string("コインは保存されません。保存する前にすべて使ってください。")); // 56
    strArray.push_back(std::string("このゲーム上の情報は自動的にサーバーに保存されません。ゲームを削除するとデータはすべて無くなります。")); // 57
    strArray.push_back(std::string("保存されたゲームを呼び出すとコインはすべて無くなります。ゲームを呼び出しますか？")); // 58
    strArray.push_back(std::string("Please Rate Us!")); // 59
}