#ifndef CHANNEL_H
#define CHANNEL_H
#include "Package.h"
#include <vector>
#include <string>
using namespace std;
/*
Klasa s�u��ca do opisu obiektu kana�u telekomunikacyjnego. Klasa ta jest niezale�na od klasy systemu telekomunikacyjnego.
Nale�y jej u�ywa� w g��wnej p�tli bezpo�rednio przez jej obiekt
*/
class Channel
{
private:
	bool free_;// Zmianna opisuj�ca zaj�to�� kana�u
	bool collision_;//Zmienna opisuj�ca wyst�pienie kolizji w kanale
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




