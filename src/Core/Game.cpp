#include <Ryu/Core/Game.h>
#include <Ryu/Core/World.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/PlayerController.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Debug/imGuiDebug.h>

// #include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>


//namespace ryu{
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
:Observer("Game")
,mWindow(sf::VideoMode(1200, 800), "SFML Application")
,mWorld(mWindow)
,mPlayerController(std::make_unique<PlayerController>(mWorld.getPlayer()))
,mIsPaused(false)
{
	// todo: how to load ichis tzextures at  startup ? 
	//mPlayer->loadTextures();
	addObservers();
}

void
Game::onNotify(const SceneNode& entity, Event event)
{
	switch(event)
	{
		case Event::DebugToggle:
		{
				RyuDebug::activateRyuDebug == false ? RyuDebug::activateRyuDebug = true : RyuDebug::activateRyuDebug = false;
				break;
		}
		case Event::ImGuiDemoToggle:
		{
				RyuDebug::showImGuiDemoWindow == false ? RyuDebug::showImGuiDemoWindow = true : RyuDebug::showImGuiDemoWindow = false;
				break;
		}
		default: break;
	}
}

void
Game::addObservers()
{
	auto player = mWorld.getPlayer();
	player->addObserver(mPlayerController.get());
	mPlayerController->addObserver(&mWorld);
	mPlayerController->addObserver(this);
}

void Game::run()
{
	sf::Clock clock;
	// uses fixed tick steps (use same delta every time)
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	ImGui::SFML::Init(mWindow);
	
	while (mWindow.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			CommandQueue& commands = mWorld.getActiveCommands();
			sf::Event event;
			
			while (mWindow.pollEvent(event))
			{
    		ImGui::SFML::ProcessEvent(mWindow, event);
				processEvents(event, commands);
			}
			
      ImGui::SFML::Update(mWindow, timeSinceLastUpdate);
			mPlayerController->handleRealtimeInput(commands);
			
			if (!mIsPaused)
			{
				update(TimePerFrame);
				RyuDebug::CreateDebugGui();
			}
		  render();
		}	
	}
	ImGui::SFML::Shutdown();
}

void Game::processEvents(sf::Event& event, CommandQueue& commands) 
{
		// Player-related one-time events
		mPlayerController->handleEvent(event, commands);

		switch(event.type)
		{		
			case sf::Event::GainedFocus:
			{
				mIsPaused = false;
				break;
			}
			case sf::Event::LostFocus:
			{
				mIsPaused = true;
				break;
			}
			case sf::Event::Closed:
			{
				mWindow.close();
				break;
			}
		}
}


void
Game::setDebugValues()
{
	auto ryustate = (mWorld.getPlayer()->getCharacterStateEnum())._to_string();
	RyuDebug::characterState = ryustate;
	// RyuDebug::characterState = ryustate;
}

void Game::update(sf::Time deltaTime)
{
	mWorld.update(deltaTime);
	setDebugValues();
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	//TODO: statistics fps text ....
 	ImGui::SFML::Render(mWindow); /// needs to be called BEFORE display()
	mWindow.display();
}
//} /// namespace ryu