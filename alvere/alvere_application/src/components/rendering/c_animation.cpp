#include <alvere/debug/logging.hpp>

#include "c_animation.hpp"

C_Animation::C_Animation()
	: m_AnimationLookup()
	, m_Animations()
	, m_CurrentAnimation(-1)
	, m_CurrentFrame(0)
	, m_CurrentRuntime(0.0f)
{
}

void C_Animation::Add(const std::string & id, const Animation & animation)
{
	auto iter = m_AnimationLookup.find(id);
	if (iter != m_AnimationLookup.end())
	{
		alvere::LogWarning("Ignoring duplicate animation with id: '%s'", id.c_str());
		return;
	}

	m_AnimationLookup.emplace(id, m_Animations.size());
	m_Animations.emplace_back(animation);
}

void C_Animation::Start(const std::string & id)
{
	auto iter = m_AnimationLookup.find(id);
	if (iter == m_AnimationLookup.end())
	{
		alvere::LogWarning("Start request can't find animation with id: '%s'", id.c_str());
		return;
	}

	m_CurrentAnimation = iter->second;
	m_CurrentFrame = 0;
	m_CurrentRuntime = 0.0f;
}

bool C_Animation::CurrentAnimationFinished() const
{
	return m_CurrentAnimation != -1
		&& m_CurrentFrame == CurrentAnimation().m_Frames.size();
}

const C_Animation::Animation & C_Animation::CurrentAnimation() const
{
	return m_Animations[m_CurrentAnimation];
}