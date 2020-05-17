#pragma once

#include <vector>
#include <unordered_map>

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

class C_Animation : public alvere::PooledComponent<C_Animation>
{
public:

	struct Frame
	{
		float m_Duration;
		alvere::Vector2i m_SpriteOffset;
	};

	struct Animation
	{
		std::vector<Frame> m_Frames;
		bool m_Loop;
	};


	std::vector<Animation> m_Animations;
	std::unordered_map<std::string, int> m_AnimationLookup;

	int m_CurrentAnimation;
	int m_CurrentFrame;
	float m_CurrentRuntime;


	C_Animation();

	void Add(const std::string & id, const Animation & animation);
	void Start(const std::string & id);

	bool CurrentAnimationFinished() const; //Will return true if no animation, will never return true if animation loops
	const Animation & CurrentAnimation() const;

};