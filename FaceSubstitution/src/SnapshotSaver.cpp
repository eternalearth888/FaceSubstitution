/*
 * SnapshotSaver.cpp
 *
 *  Created on: 04/03/2012
 *      Author: arturo
 */

#include "SnapshotSaver.h"
#include "ofImage.h"

SnapshotSaver::SnapshotSaver() {
	// TODO Auto-generated constructor stub

}

SnapshotSaver::~SnapshotSaver() {
	// TODO Auto-generated destructor stub
}



void SnapshotSaver::setup(string _folder){
	ofDirectory dir(_folder);
	if(!dir.exists()) dir.create(true);
	folder = _folder;
	ftpServer = settings.getValue("server","arturocastro.net");
	user = settings.getValue("user","user");
	password = settings.getValue("password","password");
	serverPath = settings.getValue("serverPath","serverPath");
	startThread(true,false);
}

void SnapshotSaver::save(ofPixels & _pixels){
	if(mutex.tryLock()){
		pixels = &_pixels;
		saveFrame.signal();
		unlock();
	}
}

void SnapshotSaver::threadedFunction(){
	while(isThreadRunning()){
		saveFrame.wait(mutex);
		rgbPixels.allocate(pixels->getWidth(),pixels->getHeight(),3);
		rgbPixels.setChannel(0,pixels->getChannel(0));
		rgbPixels.setChannel(1,pixels->getChannel(1));
		rgbPixels.setChannel(2,pixels->getChannel(2));
		string snapshotPath = ofFilePath::join(folder,ofGetTimestampString()+".jpg");
		ofSaveImage(rgbPixels,snapshotPath);
		system(("curl -u " + user+ ":" + password + " -T " + ofToDataPath(snapshotPath) + " ftp://" + ofFilePath::join(ftpServer , serverPath) +" & ").c_str());
	}
}
