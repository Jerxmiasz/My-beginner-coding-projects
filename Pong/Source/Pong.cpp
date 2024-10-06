#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>


int random(int x, int y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(x, y);
    return dis(gen);
}

class pong {
public:
    sf::Vector2u tamanoVentana;
    sf::RectangleShape plataformaIzquierda;
    sf::RectangleShape plataformaDerecha;
    sf::CircleShape ball;
    bool isRoundStart = true;
    double platformSpeed = 1;
    double ballSpeed = 1;
    bool playerLost = false;
    sf::Vector2f sBallDirection;
    sf::Vector2f* ballDirection = &sBallDirection;
    //Prepare enviroment function
    void prepareEnviroment(sf::RenderWindow &ventanaPrincipal){

        // Variables
        tamanoVentana = ventanaPrincipal.getSize();
        // Dibuja los elementos
        ball.setPosition(sf::Vector2f((tamanoVentana.x / 2), tamanoVentana.y / 2));
        ball.setRadius(10);

        plataformaDerecha.setSize(sf::Vector2f(15, 60));
        plataformaDerecha.setPosition(sf::Vector2f(tamanoVentana.x - plataformaDerecha.getSize().x, tamanoVentana.y / 2 - plataformaDerecha.getSize().y / 2));

        plataformaIzquierda.setSize(sf::Vector2f(15, 60));
        plataformaIzquierda.setPosition(0, 300 - plataformaIzquierda.getSize().y / 2);
    }
    //Handles the platform movements
    void handlePlatformMovement(){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {   
            //if the platform colides with the boundaries then return end
            if (plataformaIzquierda.getPosition().y <= 0)
            {
                return;
            }
            plataformaIzquierda.setPosition(sf::Vector2f(0, plataformaIzquierda.getPosition().y - platformSpeed));
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {   
            //if the platform collides with the boundaries then return end
            if (plataformaIzquierda.getPosition().y >= tamanoVentana.y - plataformaIzquierda.getSize().y)
            {
                return;
            }

            plataformaIzquierda.setPosition(sf::Vector2f(0, plataformaIzquierda.getPosition().y + platformSpeed));
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            //Handle colision
            if (plataformaDerecha.getPosition().y <= 0) { return; };

            plataformaDerecha.setPosition(sf::Vector2f(plataformaDerecha.getPosition().x, plataformaDerecha.getPosition().y - platformSpeed));
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            //Handle colision
            if (plataformaDerecha.getPosition().y >= tamanoVentana.y - plataformaDerecha.getSize().y){
                return;
            };

            plataformaDerecha.setPosition(sf::Vector2f(plataformaDerecha.getPosition().x, plataformaDerecha.getPosition().y + platformSpeed));

        }


    }

    void handleBallMovement()
    {
        if (isRoundStart)
        {
            int randomDirection = random(0, 3);
            if (randomDirection == 0)
            {
                *ballDirection = sf::Vector2f(-1, 1);
            }
            else if (randomDirection == 1)
            {
                *ballDirection = sf::Vector2f(1, 1);
            }
            else if (randomDirection == 2)
            {
                *ballDirection = sf::Vector2f(1, -1);
            }
            else if (randomDirection == 3)
            {
                *ballDirection = sf::Vector2f(-1,-1);
            }
            isRoundStart = false;
        };

        ball.move(ballDirection->x * ballSpeed, ballDirection->y * ballSpeed);

        if (ball.getPosition().y <= 0 || ball.getPosition().y + ball.getRadius() * 2 >= tamanoVentana.y)
        {
            ballDirection->y = -ballDirection->y; // Reverse the y direction
        }

        if (ball.getPosition().x <= 0 || ball.getPosition().x >= 781)
        {
            playerLost = true;
        }

        // Check for collision with the left and right of the window
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= tamanoVentana.x)
        {
            ballDirection->x = -ballDirection->x; // Reverse the x direction
        }

        // Check for collision with the platforms
        if (ball.getGlobalBounds().intersects(plataformaIzquierda.getGlobalBounds()) || ball.getGlobalBounds().intersects(plataformaDerecha.getGlobalBounds()))
        {
            ballDirection->x = -ballDirection->x; // Reverse the x direction
        }
    }
    void drawEnviroment(sf::RenderWindow &ventanaPrincipal)
    {   

        ventanaPrincipal.draw(plataformaIzquierda);
        ventanaPrincipal.draw(plataformaDerecha);
        ventanaPrincipal.draw(ball);
    }
};

  ///////////////////////////////////////////////////////////////////
 /////////////////////////MAIN FUNCTION/////////////////////////////
///////////////////////////////////////////////////////////////////

int main()
{   

    sf::RenderWindow ventanaPrincipal(sf::VideoMode(800,600), "Pong!");
    ventanaPrincipal.setFramerateLimit(60);
    pong game;
    game.prepareEnviroment(ventanaPrincipal);
    game.platformSpeed = 3.0;
    game.ballSpeed = 3.0;
    
    sf::Clock clock;
    while (ventanaPrincipal.isOpen())
    {   
        //Manage close event
        sf::Event evento;
        while (ventanaPrincipal.pollEvent(evento))
        {
            if (evento.type == sf::Event::Closed)
            {
                ventanaPrincipal.close();
            }
        }


        game.handlePlatformMovement();
        game.handleBallMovement();
        if (game.playerLost == true)
        {   
            game.prepareEnviroment(ventanaPrincipal);
            game.playerLost = false;
            game.isRoundStart = true;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            game.handlePlatformMovement();
            game.handleBallMovement();
        }

        ventanaPrincipal.clear(sf::Color(0, 0, 0));
        game.drawEnviroment(ventanaPrincipal);
        ventanaPrincipal.display();
    }
    return 0;
    
}
