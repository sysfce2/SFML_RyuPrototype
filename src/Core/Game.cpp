#include <Ryu/Core/Game.h>
#include <Ryu/Core/World.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/PlayerController.h>
#include <Ryu/Control/CharacterEnums.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>


//namespace ryu{
const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(1200, 800), "SFML Application")
,mWorld(mWindow)
,mPlayerController(std::make_unique<PlayerController>(mWorld.getPlayer()))
,mIsPaused(false)
{
	// todo: how to load ichis tzextures at  startup ? 
	//mPlayer->loadTextures();
	addObservers();
}

void
Game::addObservers()
{
	auto player = mWorld.getPlayer();
	player->addObserver(mPlayerController.get());
	mPlayerController->addObserver(&mWorld);
}

void Game::run()
{
	sf::Clock clock;
	// uses fixed tick steps (use same delta every time)
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	ImGui::SFML::Init(mWindow);
	
	// TODO: why no debugdrawing ?????
	//mWorld.setDebugDrawer(mWindow);

	while (mWindow.isOpen())
	{
		//processEvents();
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
			
      ImGui::SFML::Update(mWindow, timeSinceLastUpdate);//clock.restart());
			mPlayerController->handleRealtimeInput(commands);
			
			if (!mIsPaused)
			{
				//ImGui::NewFrame();
				update(TimePerFrame);
				// ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();
			}
	  	ImGui::SFML::Render(mWindow);
		}	
		
		render();
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

void Game::update(sf::Time deltaTime)
{
	mWorld.update(deltaTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	//TODO: statistics fps text ....
	mWindow.display();
}
//} /// namespace ryu