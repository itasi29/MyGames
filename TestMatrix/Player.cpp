#include "Player.h"
#include <string>
#include "Common/Input.h"

Player::Player() :
	MyEngine::Collidable(MyEngine::Collidable::Priority::High, ObjectTag::Player, MyEngine::ColliderBase::Kind::Sphere)
{
	m_colliderSphere = dynamic_cast<MyEngine::ColliderSphere*>(m_collider.get());
	m_colliderSphere->radius = 5.0f;
	m_rigid.SetPos(MyEngine::Vec3());
}

Player::~Player()
{
}

void Player::Update()
{
	auto& input = Input::GetInstance();
	MyEngine::Vec3 velocity;

	if (input.IsPress(0, "up"))
	{
		++velocity.y;
	}
	if (input.IsPress(0, "down"))
	{
		--velocity.y;
	}
	if (input.IsPress(0, "left"))
	{
		--velocity.x;
	}
	if (input.IsPress(0, "right"))
	{
		++velocity.x;
	}

	velocity.Normalize();
	velocity *= 1.0f;

	m_rigid.SetVelocity(velocity);
}

void Player::OnCollideEnter(const Collidable& colider)
{
	auto tag = colider.GetTag();

	std::wstring message = L"Player‚ª";

	if (tag == ObjectTag::Player) 
	{
		message += L"Player";
	}
	else if (tag == ObjectTag::Enemy)
	{
		message += L"Enemy";
	}

	message += L"‚Æ“–‚½‚Á‚½(Õ“Ë)\n";
	printfDx(message.c_str());
}

void Player::OnCollideStay(const Collidable& colider)
{
	auto tag = colider.GetTag();

	std::wstring message = L"Player‚ª";

	if (tag == ObjectTag::Player)
	{
		message += L"Player";
	}
	else if (tag == ObjectTag::Enemy)
	{
		message += L"Enemy";
	}

	message += L"‚Æ“–‚½‚Á‚Ä‚Ü‚·\n";
	printfDx(message.c_str());
}

void Player::OnCollideExit(const Collidable& colider)
{
	auto tag = colider.GetTag();

	std::wstring message = L"Player‚ª";

	if (tag == ObjectTag::Player)
	{
		message += L"Player";
	}
	else if (tag == ObjectTag::Enemy)
	{
		message += L"Enemy";
	}

	message += L"‚Æ—£‚ê‚½\n";
	printfDx(message.c_str());
}
