//
//  Archive.cpp
//  RGAssignment3
//
//  Created by rick gessner on 1/24/21.
//

#include "Archive.hpp"

namespace ECE141 {

	//STUDENT put archive class code here... 
	void ArchiveObserver::operator()(ActionType anAction,
		const std::string& aName, bool status) {
		ActionTypeList.push_back(anAction);
		observerName = aName;
		observerStatus = status;
	}

	Archive::Archive(const std::string& aFullPath, AccessMode aMode) {
		theArchiveName = aFullPath;
		thisMode = aMode;
	}


	Archive& Archive::addObserver(ArchiveObserver& anObserver) {
		theObserver = &anObserver;
		return *this;
	}
	
	bool Archive::extract(const std::string& aFilename, const std::string& aFullPath) {
		std::fstream theFile(aFullPath, std::ios::out | std::ios::binary);
		Block newBlock;
		int cur = archiveStream.tellg();
		archiveStream.seekg(0, std::ios::end);
		int fileSize = archiveStream.tellg();
		//while ()
		while (cur < fileSize) {
			archiveStream.seekg(cur);
			//int end = theFine.endFile
			archiveStream.read(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(newBlock), fileSize - cur));
			if (newBlock.title == aFilename) {
				theFile.write(reinterpret_cast<const char*>(&newBlock), sizeof(newBlock));
			}
			cur += std::min((int)sizeof(newBlock), fileSize - cur);
		}
		return true;
	}

	
	bool Archive::remove(const std::string& aFilename) {

		Block newBlock;
		archiveStream.seekg(0);
		int cur = archiveStream.tellg();
		archiveStream.seekg(0, std::ios::end);
		int fileSize = archiveStream.tellg();
		//while ()
		while (cur < fileSize) {
			archiveStream.seekg(cur);
			//int end = theFine.endFile
			archiveStream.read(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(newBlock), fileSize - cur));
			if (newBlock.title == aFilename) {
				//archiveStream.write(reinterpret_cast<const char*>(&newBlock), sizeof(newBlock));
				// set the block empty
				newBlock.available = true;
			}
			archiveStream.seekg(cur);
			archiveStream.write(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(newBlock), fileSize - cur));
			cur += std::min((int)sizeof(newBlock), fileSize - cur);
		}
		debugDump(std::cout);
		return true;
	}

	size_t Archive::list(std::ostream& aStream) {
		int fileCount = 0;
		std::string fileName = "";
		Block newBlock;
		archiveStream.seekg(0);
		int cur = archiveStream.tellg();
		archiveStream.seekg(0, std::ios::end);
		int fileSize = archiveStream.tellg();
		//while ()
		aStream << "# " << "Name\n";
		aStream << "----------------\n";
		while (cur < fileSize) {
			archiveStream.seekg(cur);
			//int end = theFine.endFile
			archiveStream.read(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(Block), fileSize - cur));
			if (fileName != newBlock.title) {
				if (newBlock.available == false) {
					fileName = newBlock.title;
					fileCount += 1;

					aStream << fileCount << " " << fileName << "\n";
				}

			}
			cur += std::min((int)sizeof(Block), fileSize - cur);
		}
		return fileCount;
	}

	size_t Archive::debugDump(std::ostream& aStream) {
		int blockCount = 0;
		std::string fileName = "";
		Block newBlock;
		archiveStream.seekg(0);
		int cur = archiveStream.tellg();
		archiveStream.seekg(0, std::ios::end);
		int fileSize = archiveStream.tellg();
		//while ()
		aStream << "###" << " status " << " name " << "\n";
		while (cur < fileSize) {
			archiveStream.seekg(cur);
			//int end = theFine.endFile
			archiveStream.read(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(Block), fileSize - cur));
			if (newBlock.available == false) {
				fileName = newBlock.title;
				aStream << blockCount  + 1 << " used " << fileName << "\n";	
				//std::cout << blockCount + 1 << " used " << fileName << "\n";
			}
		    else {
				aStream << blockCount + 1 << " empty \n";
		    }
			blockCount += 1;
			cur += std::min((int)sizeof(Block), fileSize - cur);
		}
		return blockCount;
	}

	size_t Archive::compact() {
		//Archive* newArchive = createArchive(theArchiveName);
		std::fstream temp("temp", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		//int curNew = temp.tellg();
		int exsitingBlockCount = 0;
		std::string fileName = "";
		Block newBlock;
		archiveStream.seekg(0);
		int cur = archiveStream.tellg();
		archiveStream.seekg(0, std::ios::end);
		int fileSize = archiveStream.tellg();
		//while ()
		while (cur < fileSize) {
			archiveStream.seekg(cur);
			//temp.seekg(curNew);
			archiveStream.read(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(Block), fileSize - cur));
			if (newBlock.available == false) {
				exsitingBlockCount += 1;
				temp.write(reinterpret_cast<char*>(&newBlock), std::min((int)sizeof(Block), fileSize - cur));
				//std::cout << blockCount + 1 << " used " << fileName << "\n";
				//curNew += std::min((int)sizeof(Block), fileSize - cur);
			}
			cur += std::min((int)sizeof(Block), fileSize - cur);
		}
		//this = Archive(newArchive);
		archiveStream.close();
		temp.close();
		std::filesystem::rename("temp", theArchiveName);
		archiveStream.open(theArchiveName, std::ios::in | std::ios::out | std::ios::binary);
		return exsitingBlockCount;
	}
	std::string Archive::getFullPath() const { //get archive path (including .arc extension) 
		return theArchiveName;
	}
}
