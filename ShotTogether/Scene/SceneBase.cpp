#include "SceneBase.h"

SceneBase::SceneBase(SceneManager& scnMgr, FileManager& fileMgr, SoundSystem& sndSys, Camera& camera) :
	m_scnMgr(scnMgr),
	m_fileMgr(fileMgr),
	m_sndSys(sndSys),
	m_camera(camera)
{
}
