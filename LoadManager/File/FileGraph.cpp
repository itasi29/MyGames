#include "FileGraph.h"

#include <DxLib.h>
#include "FileManager.h"

FileGraph::FileGraph(FileManager& mgr) :
	FileBase(mgr)
{
}

FileGraph::~FileGraph()
{
}

void FileGraph::Delete()
{
	DeleteGraph(m_handle);
}
