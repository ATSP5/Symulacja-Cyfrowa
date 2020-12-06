#ifndef CHANNEL_H
#define CHANNEL_H
#include "Package.h"
#include <vector>
#include <string>
using namespace std;
/*
Klasa s³u¿¹ca do opisu obiektu kana³u telekomunikacyjnego. Klasa ta jest niezale¿na od klasy systemu telekomunikacyjnego.
Nale¿y jej u¿ywaæ w g³ównej pêtli bezpoœrednio przez jej obiekt
*/
class Channel
{
private:
	bool free_;// Zmianna opisuj¹ca zajêtoœæ kana³u
	bool collision_;//Zmienna opisuj¹ca wyst¹pienie kolizji w kanale
	vector<Package* > package_buffer_;
public:
	 Channel();
	 ~Channel();
	 //Get methods:
	 bool GetChannelStatus();
	 bool GetCollisionFlag();
	 vector<Package* > GetChannelPackageBuffer();
	 string GetChannelOcupyStatusAsString();
	 //Set Methods:
	 void SetChannelStatus(bool status_);
	 void SetCollisionIncydent(bool incydent_);
	 void CopyPackageToBuffer(Package* package_buffer_V);
	 void EreasePackageFromBufferAtIndex(int index);
	 void PopBackFromPackageBuffer();
};

#endif // !Channel




