//Robota Serial Protocol


unsigned char sendPkg(char read,char id,char arg);
serialArduino mega;

void setVW(float v,float w){
	if(v>0){
		sendPkg(0,1,abs(v*133));
		sendPkg(0,2,0);
	}
	else{
		sendPkg(0,1,abs(v*133));
		sendPkg(0,2,1);
	}

	if(w>0){
		sendPkg(0,3,abs(w*21.7));
		sendPkg(0,4,0);
	}
	else{
		sendPkg(0,3,abs(w*21.7));
		sendPkg(0,4,1);
	}
}

unsigned char sendPkg(char read,char id,char arg){
	mega.sendByte(255);
    mega.sendByte(254);
    mega.sendByte(read); //read
    mega.sendByte(id); //id
    mega.sendByte(arg); //arg
    mega.sendByte(253);
	return mega.reciveByte();	
}

