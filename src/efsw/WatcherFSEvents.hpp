#ifndef EFSW_WATCHERINOTIFY_HPP
#define EFSW_WATCHERINOTIFY_HPP

#include <efsw/FileWatcherImpl.hpp>

#if EFSW_PLATFORM == EFSW_PLATFORM_FSEVENTS

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <efsw/FileInfo.hpp>
#include <efsw/WatcherGeneric.hpp>
#include <set>
#include <vector>

namespace efsw {

class FileWatcherFSEvents;

class FSEvent {
  public:
	FSEvent( std::string path, long flags, Uint64 id, Uint64 inode = 0 ) :
		Path( path ), Flags( flags ), Id( id ), inode( inode ) {}

	std::string Path;
	long Flags;
	Uint64 Id;
	Uint64 inode{ 0 };
};

class WatcherFSEvents : public Watcher {
  public:
	WatcherFSEvents();

	WatcherFSEvents( WatchID id, std::string directory, FileWatchListener* listener, bool recursive,
					 WatcherFSEvents* parent = NULL );

	~WatcherFSEvents();

	void init();

	void handleActions( std::vector<FSEvent>& events );

	void process();

	Atomic<FileWatcherFSEvents*> FWatcher;
	FSEventStreamRef FSStream;

  protected:
	void handleAddModDel( const Uint32& flags, const std::string& path, std::string& dirPath,
						  std::string& filePath );

	WatcherGeneric* WatcherGen;

	std::set<std::string> DirsChanged;

	void sendFileAction( WatchID watchid, const std::string& dir, const std::string& filename,
						 Action action, std::string oldFilename = "" );
};

} // namespace efsw

#endif

#endif
