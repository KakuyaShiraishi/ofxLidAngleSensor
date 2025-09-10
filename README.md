# ofxLidAngleSensor


MacBook の「Lid Angle（画面ヒンジ角）」を IOKit/HID の Feature Report から読み出す openFrameworks アドオン。

本アドオンは Sam Henri Gold 氏のリバースエンジニアリング成果（[LidAngleSensor](https://github.com/samhenrigold/LidAngleSensor)）と、派生解説（[pybooklid](https://github.com/tcsenpai/pybooklid)）の知見（Feature Report/Report ID/データ形式）に基づいて仕様面の参照を行っています。ソースコードは独自実装です。ライセンスはApache-2.0で提供し、NOTICEにクレジットを明記しています。

- OS: macOS (Apple Silicon/Intel)
- openFrameworks: 0.12.x+