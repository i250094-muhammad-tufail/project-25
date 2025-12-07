#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/grid.h"
#include "../core/switches.h"
#include "../core/io.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>

// ============================================================================
// APP.CPP - Implementation of SFML application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES FOR APP STATE
// ----------------------------------------------------------------------------
static sf::RenderWindow* g_window = nullptr;
static sf::Font g_font;

// View for camera (panning/zoom)
static sf::View g_camera;

// Simulation state
static bool g_isPaused = false;
static bool g_isStepMode = false;

// Mouse state
static bool g_isDragging = false;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

// Grid rendering parameters
static float g_cellSize = 40.0f;
static float g_gridOffsetX = 50.0f;
static float g_gridOffsetY = 50.0f;

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// This function will initialize the SFML application window and resources.
// It creates a render window with a specified size and title, sets the
// framerate limit, attempts to load a font file for text rendering, and
// initializes the camera view. Returns true on success, false on failure.
// This should be called once at the start of the application before entering
// the main loop.
// ----------------------------------------------------------------------------
bool initializeApp() {
g_window = new sf::RenderWindow(sf::VideoMode(1280,720),"Switchback Rails");
g_window->setFramerateLimit(60);
return true;
}


// ----------------------------------------------------------------------------
// MAIN RUN LOOP
// ----------------------------------------------------------------------------
// This function will run the main application loop. It handles event processing,
// simulation updates, and rendering. The loop continues while the window is open.
// It processes SFML events (window close, keyboard input, mouse input), updates
// the simulation at a fixed interval (2 ticks per second) when not paused,
// checks if the simulation is complete, and renders the current frame. Keyboard
// controls: SPACE to pause/resume, PERIOD to step one tick, ESC to exit. The
// loop exits when the window is closed or ESC is pressed.
// ----------------------------------------------------------------------------
void runApp() {
sf::Clock clock;
float timer=0.0f;
while(g_window->isOpen())
{
    sf::Event event;
    while(g_window->pollEvent(event))
    {
    if(event.type ==sf::Event::Closed)g_window->close();
    if(event.type ==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Space) 
    g_isPaused = !g_isPaused;
    }
    if(!g_isPaused && (timer += clock.restart().asSeconds()) >=0.5f)
    {
        simulateOneTick();
        timer=0.0f;
    }
    else if(g_isPaused)
    clock.restart();
    g_window->clear(sf::Color(20,20,20));
    for(int y = 0 ; y<gridHeight; y++)
    {
        for(int x=0;x<gridWidth;x++)
        {
            char c=Grid[y][x];
            if(c==' ')
            continue;
            sf::RectangleShape tile(sf::Vector2f(38,38));
            tile.setPosition(x*40+1,y*40+1);
            if (c == 'S')
            {
                tile.setFillColor(sf::Color(100, 255, 100));
            }
            else if (c == 'D')
            {
                tile.setFillColor(sf::Color(100, 100, 255));

            }
            else if (c>='A' && c<= 'Z')
            {
                tile.setFillColor(sf::Color(200, 200, 0));
            }
            else if (c == '=')
            {
                tile.setFillColor(sf::Color(255, 150, 0));

            }
            else
            {
                tile.setFillColor(sf::Color(50, 50, 50));
            }
            g_window->draw(tile);
        }
    }
    for(int i=0;i<trainSpawned;i++)
    {
        if(trainState[i]==activeTrain)
        {
            sf::CircleShape train(13);
            train.setPosition(trainPosition_x[i]*40+13,trainPosition_y[i]*40+13);
            train.setFillColor(sf::Color::Magenta);
            g_window->draw(train);
        }
        else if(trainState[i]==waitingTrain)
        {
            sf::CircleShape train(13);
            train.setPosition(trainPosition_x[i]*40+13,trainPosition_y[i]*40+13);
            train.setFillColor(sf::Color(255,100,100));
            g_window->draw(train);
        }
    }
    g_window->display();
    }

    }

    //------------------------------------------------------------------------
// CLEANUP
// ----------------------------------------------------------------------------
// This function will clean up all resources and close the application window.
// It deletes the render window object and sets the pointer to nullptr. This
// should be called once at the end of the application before exiting to ensure
// proper resource cleanup.
// ----------------------------------------------------------------------------
void cleanupApp() {
if(g_window)
{
    delete g_window;
    g_window=nullptr;
}}
