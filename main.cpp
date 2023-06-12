#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

const float PI = 3.14159265;

const sf::Vector2i WINDOW_SIZE(800, 600);
const bool VSYNC = true;

const char FONT_PATH[] = "fonts/Roboto-Regular.ttf";

const int UNIT_CIRCLE_RADIUS = 150;
const float WHEEL_STRENGTH = 5;

void WithTrigFunctions();
void WithoutTrigFunctions();

int main() {
    WithTrigFunctions();
    //WithoutTrigFunctions();
}

void WithoutTrigFunctions() {
    sf::RenderWindow window;
    // create the window with size, title and style
    window.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Window", sf::Style::None);

    // activates verticalsync
    window.setVerticalSyncEnabled(VSYNC);

    sf::Vector2f center = sf::Vector2f(WINDOW_SIZE.x/2, WINDOW_SIZE.y/2);

    // unit circle
    sf::CircleShape unitCircle(UNIT_CIRCLE_RADIUS);
    unitCircle.setPosition(center.x - UNIT_CIRCLE_RADIUS, center.y - UNIT_CIRCLE_RADIUS);
    unitCircle.setFillColor(sf::Color::Black);
    unitCircle.setOutlineColor(sf::Color(100, 100, 100));
    unitCircle.setOutlineThickness(1);

    float pointRadius = 5;
    sf::CircleShape unitCirclePoint(pointRadius);
    unitCirclePoint.setFillColor(sf::Color::White);

    sf::Vector2f windowMousePosition;
    sf::Vector2f calcMousePosition;

    // lines
    sf::Vertex tangentLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(0, center.y))
    };
    sf::Vertex sineLine[] =
    {
        sf::Vertex(sf::Vector2f(0, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    sf::Vertex cosineLine[] =
    {
        sf::Vertex(sf::Vector2f(0, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };

    // start window loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed :
                    window.close();
                    break;
                
                default:
                    break;
            }
        }

        // get inputs and center on window
        windowMousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
        calcMousePosition = windowMousePosition - center;

        // get position in circle
        sf::Vector2f unitPosition = sf::Vector2f(0, 0);

        float b_a = calcMousePosition.y/calcMousePosition.x;
        unitPosition.x = 1/(sqrt(pow(b_a, 2) + 1));
        unitPosition.y = b_a * unitPosition.x;

        unitPosition = calcMousePosition.x > 0 ? sf::Vector2f(unitPosition.x, unitPosition.y) : -sf::Vector2f(unitPosition.x, unitPosition.y);

        // set sine cosine
        float sine = unitPosition.y;
        float cosine = unitPosition.x;
        
        // calc tangent points
        sf::Vector2f tangentIntersections = sf::Vector2f(0, 0);

        tangentIntersections.y = pow(cosine, 2)/sine + sine; // == 1/sine
        tangentIntersections.x = pow(sine, 2)/cosine + cosine; // == 1/cosine

        // set tan cot
        float tangent = sqrt(pow(cosine - tangentIntersections.x, 2) + pow(sine,2)); // == sine/cosine
        float cotangent = sqrt(pow(sine - tangentIntersections.y, 2) + pow(cosine,2)); // == cosine/sine

        // set sec csc
        float secant = tangentIntersections.x;
        float cosecant = tangentIntersections.y;

        window.clear(sf::Color::Black);
        
        // set lines
        tangentLine[0].position.y = tangentIntersections.y * (float)UNIT_CIRCLE_RADIUS + center.y;
        tangentLine[1].position.x = tangentIntersections.x * (float)UNIT_CIRCLE_RADIUS + center.x;
        tangentLine[0].color = sf::Color(0, 255, 0);
        tangentLine[1].color = tangentLine[0].color;

        sineLine[0].position.x = cosine * (float)UNIT_CIRCLE_RADIUS + center.x;
        sineLine[1].position.x = cosine  * (float)UNIT_CIRCLE_RADIUS + center.x;
        sineLine[0].position.y = sine  * (float)UNIT_CIRCLE_RADIUS + center.y;
        sineLine[1].position.y = center.y;
        sineLine[0].color = sf::Color(255, 0, 0);
        sineLine[1].color = sineLine[0].color;

        cosineLine[0].position.y = sine * (float)UNIT_CIRCLE_RADIUS + center.y;
        cosineLine[1].position.y = sine  * (float)UNIT_CIRCLE_RADIUS + center.y;
        cosineLine[0].position.x = cosine  * (float)UNIT_CIRCLE_RADIUS + center.x;
        cosineLine[1].position.x = center.x;
        cosineLine[0].color = sf::Color(0, 0, 255);
        cosineLine[1].color = cosineLine[0].color;

        unitCirclePoint.setPosition(unitPosition * (float)UNIT_CIRCLE_RADIUS - sf::Vector2f(pointRadius, pointRadius) + center);

        // drawing
        window.draw(unitCircle);

        window.draw(tangentLine, 2, sf::Lines);
        window.draw(sineLine, 2, sf::Lines);
        window.draw(cosineLine, 2, sf::Lines);

        window.draw(unitCirclePoint);

        // display
        window.display();
    }
}

void WithTrigFunctions() {
    sf::RenderWindow window;
    // create the window with size, title and style
    window.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Window", sf::Style::None);

    // activates verticalsync
    window.setVerticalSyncEnabled(VSYNC);

    sf::Font font;
    // load font from fonts file
    if (!font.loadFromFile(FONT_PATH))
    {
        std::cout << "Font error" << std::endl;
    }
    
    // setup text labels
    sf::Text textLabels[11];

    for (int i = 0; i < 11; i++) {
        textLabels[i].setFont(font);
        textLabels[i].setCharacterSize(24);
        textLabels[i].setFillColor(sf::Color::White);
        textLabels[i].setPosition(sf::Vector2f(0, i*24));

        textLabels[i].setString("aaaa");
    }


    sf::Vector2i mousePosition;
    int mouseWheelOffset = 0;
    int SEC_CSC_OFFSET = (UNIT_CIRCLE_RADIUS + mouseWheelOffset) / 8;
    sf::Vector2f center = sf::Vector2f(WINDOW_SIZE.x/2, WINDOW_SIZE.y/2);


    // cartesian lines
    sf::Vertex xLine[] =
    {
        sf::Vertex(sf::Vector2f(0.f, center.y), sf::Color(100, 100, 100)),
        sf::Vertex(sf::Vector2f(WINDOW_SIZE.x, center.y), sf::Color(100, 100, 100))
    };
    sf::Vertex yLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 1), sf::Color(100, 100, 100)),
        sf::Vertex(sf::Vector2f(center.x, WINDOW_SIZE.y), sf::Color(100, 100, 100))
    };

    // unit circle
    sf::CircleShape unitCircle((UNIT_CIRCLE_RADIUS + mouseWheelOffset));
    unitCircle.setPosition(center.x - (UNIT_CIRCLE_RADIUS + mouseWheelOffset), center.y - (UNIT_CIRCLE_RADIUS + mouseWheelOffset));
    unitCircle.setFillColor(sf::Color::Black);
    unitCircle.setOutlineColor(sf::Color(100, 100, 100));
    unitCircle.setOutlineThickness(1);

    // mouse line
    sf::Vertex mouseLine[] =
    {
        sf::Vertex(center),
        sf::Vertex(sf::Vector2f(0, 0))
    };

    // sin lines
    sf::Vertex sinLine1[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    sf::Vertex sinLine2[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // cos lines
    sf::Vertex cosLine1[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    sf::Vertex cosLine2[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // tan line
    sf::Vertex tanLine[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // cot line
    sf::Vertex cotLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // sec line
    sf::Vertex secLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // csc line
    sf::Vertex cscLine[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, 0))
    };
    // vsn line
    sf::Vertex vsnLine[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, center.y))
    };
    // cvs line
    sf::Vertex cvsLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(center.x, 0))
    };
    // exsec line
    sf::Vertex exsecLine[] =
    {
        sf::Vertex(sf::Vector2f(0, center.y)),
        sf::Vertex(sf::Vector2f(0, center.y))
    };
    // excsc line
    sf::Vertex excscLine[] =
    {
        sf::Vertex(sf::Vector2f(center.x, 0)),
        sf::Vertex(sf::Vector2f(center.x, 0))
    };

    // start window loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;

                case sf::Event::MouseWheelScrolled :
                    mouseWheelOffset += event.mouseWheelScroll.delta * WHEEL_STRENGTH;
                    if (UNIT_CIRCLE_RADIUS + mouseWheelOffset < 2) mouseWheelOffset = 2 - UNIT_CIRCLE_RADIUS;
                    else if (UNIT_CIRCLE_RADIUS + mouseWheelOffset > 350) mouseWheelOffset = 350 - UNIT_CIRCLE_RADIUS;
                    break;

                default:
                    break;
            }
        }

        // clear screen
        window.clear(sf::Color::Black);

        // update
        unitCircle.setRadius(UNIT_CIRCLE_RADIUS + mouseWheelOffset);
        unitCircle.setPosition(center.x - (UNIT_CIRCLE_RADIUS + mouseWheelOffset), center.y - (UNIT_CIRCLE_RADIUS + mouseWheelOffset));
        //unitCircle.setFillColor(sf::Color::Black);
        //unitCircle.setOutlineColor(sf::Color(100, 100, 100));
        //unitCircle.setOutlineThickness(1);

        mousePosition = sf::Mouse::getPosition(window);
        mousePosition -= (sf::Vector2i)center;

        float angle = atan((float)mousePosition.y/mousePosition.x);

        sf::Vector2f pointInUnitCircle = sf::Vector2f(cos(angle)* (UNIT_CIRCLE_RADIUS + mouseWheelOffset), sin(angle)* (UNIT_CIRCLE_RADIUS + mouseWheelOffset)) + center;
        if (mousePosition.x < 0) {
            pointInUnitCircle = -pointInUnitCircle + center + center;
        }

        mouseLine[1] = pointInUnitCircle;

        // sin line
        sinLine1[1].position = pointInUnitCircle;
        sinLine1[0].position.x = pointInUnitCircle.x;
        sinLine1[0].color = sf::Color(255, 20, 20);
        sinLine1[1].color = sf::Color(255, 20, 20);

        sinLine2[0].color = sinLine1[0].color;
        sinLine2[1].color = sinLine1[1].color;
        
        sinLine2[0].position.y = sinLine1[0].position.y;
        sinLine2[1].position.y = sinLine1[1].position.y;

        sinLine2[0].position.x = center.x;
        sinLine2[1].position.x = center.x;


        // cos line
        cosLine1[1].position = pointInUnitCircle;
        cosLine1[0].position.y = pointInUnitCircle.y;
        cosLine1[0].color = sf::Color(12, 12, 255);
        cosLine1[1].color = sf::Color(12, 12, 255);

        cosLine2[0].color = cosLine1[0].color;
        cosLine2[1].color = cosLine1[1].color;
        
        cosLine2[0].position.x = cosLine1[0].position.x;
        cosLine2[1].position.x = cosLine1[1].position.x;

        cosLine2[0].position.y = center.y;
        cosLine2[1].position.y = center.y;

        // tan line
        tanLine[1].position = pointInUnitCircle;
        tanLine[0].position.x = mousePosition.x > 0 ? sqrt(1 + pow(tan(angle), 2)) * (UNIT_CIRCLE_RADIUS + mouseWheelOffset) + center.x : -sqrt(1 + pow(tan(angle), 2)) * (UNIT_CIRCLE_RADIUS + mouseWheelOffset) + center.x;
        tanLine[0].color = sf::Color(211, 181, 141);
        tanLine[1].color = sf::Color(211, 181, 141);

        // cot line
        cotLine[1].position = pointInUnitCircle;
        cotLine[0].position.y = mousePosition.y > 0 ? sqrt(1 + pow(1/tan(angle), 2)) * (UNIT_CIRCLE_RADIUS + mouseWheelOffset) + center.y : -sqrt(1 + pow(1/tan(angle), 2)) * (UNIT_CIRCLE_RADIUS + mouseWheelOffset) + center.y;
        cotLine[0].color = sf::Color(255,142,2);
        cotLine[1].color = sf::Color(255,142,2);

        // sec line
        secLine[1].position.x = tanLine[0].position.x;
        secLine[1].position.y = mousePosition.y > 0 ? center.y - SEC_CSC_OFFSET : center.y + SEC_CSC_OFFSET;

        secLine[0].position.y = secLine[1].position.y;
        secLine[0].color = sf::Color(0,140,140);
        secLine[1].color = sf::Color(0,140,140);

        // csc line
        cscLine[1].position.y = cotLine[0].position.y;
        cscLine[1].position.x = mousePosition.x > 0 ? center.x - SEC_CSC_OFFSET : center.x + SEC_CSC_OFFSET;

        cscLine[0].position.x = cscLine[1].position.x;
        cscLine[0].color = sf::Color(255,193,204);
        cscLine[1].color = sf::Color(255,193,204);

        // vsn line
        vsnLine[0].position.x = mousePosition.x > 0 ? center.x + (UNIT_CIRCLE_RADIUS + mouseWheelOffset) : center.x - (UNIT_CIRCLE_RADIUS + mouseWheelOffset);
        vsnLine[1].position.x = pointInUnitCircle.x;
        vsnLine[0].color = sf::Color(27,143,27);
        vsnLine[1].color = sf::Color(27,143,27);

        // cvs line
        cvsLine[0].position.y = mousePosition.y > 0 ? center.y + (UNIT_CIRCLE_RADIUS + mouseWheelOffset) : center.y - (UNIT_CIRCLE_RADIUS + mouseWheelOffset);
        cvsLine[1].position.y = pointInUnitCircle.y;
        cvsLine[0].color = sf::Color(0,255,255);
        cvsLine[1].color = sf::Color(0,255,255);

        // exsec line
        exsecLine[0].position.x = mousePosition.x > 0 ? center.x + (UNIT_CIRCLE_RADIUS + mouseWheelOffset) : center.x - (UNIT_CIRCLE_RADIUS + mouseWheelOffset);
        exsecLine[1].position.x = secLine[1].position.x;
        exsecLine[0].color = sf::Color(255,121,189);
        exsecLine[1].color = sf::Color(255,121,189);


        // excsc line
        excscLine[0].position.y = mousePosition.y > 0 ? center.y + (UNIT_CIRCLE_RADIUS + mouseWheelOffset) : center.y - (UNIT_CIRCLE_RADIUS + mouseWheelOffset);
        excscLine[1].position.y = cscLine[1].position.y;
        excscLine[0].color = sf::Color(0,100,0);
        excscLine[1].color = sf::Color(0,100,0);

        // text
        float textAngle;

        if (mousePosition.x > 0 && mousePosition.y < 0 ) { // quad 1
            textAngle = -angle;
        } else if (mousePosition.x < 0 && mousePosition.y < 0 ) { // quad 2
            textAngle = PI - angle;
        } else if (mousePosition.x < 0 && mousePosition.y > 0 ) { // quad 3
            textAngle = PI - angle;
        } else if (mousePosition.x > 0 && mousePosition.y > 0 ) { // quad 4
            textAngle = PI - angle + PI;
        }

        textLabels[0].setString("Angle: " + std::to_string(textAngle*360/(2*PI)));
        textLabels[0].setFillColor(sf::Color::White);

        textLabels[1].setString("Sin: " + std::to_string(sin(textAngle)) );
        textLabels[1].setFillColor(sf::Color(255, 20, 20));
        textLabels[2].setString("Cos: " + std::to_string(cos(textAngle)) );
        textLabels[2].setFillColor(sf::Color(12, 12, 255));

        textLabels[3].setString("Tan: " + std::to_string(tan(textAngle)) );
        textLabels[3].setFillColor(sf::Color(211, 181, 141));
        textLabels[4].setString("Cot: " + std::to_string(1/tan(textAngle)) );
        textLabels[4].setFillColor(sf::Color(255,142,2));

        textLabels[5].setString("Sec: " + std::to_string(1 + pow(tan(textAngle), 2)) );
        textLabels[5].setFillColor(sf::Color(0,140,140));
        textLabels[6].setString("Csc: " + std::to_string(1 + pow(1/tan(textAngle), 2)) );
        textLabels[6].setFillColor(sf::Color(255,193,204));

        textLabels[7].setString("Vsn: " + std::to_string(1 - cos(textAngle)) );
        textLabels[7].setFillColor(sf::Color(27,143,27));
        textLabels[8].setString("Cvs: " + std::to_string(1 - sin(textAngle)) );
        textLabels[8].setFillColor(sf::Color(0,255,255));

        textLabels[9].setString("Exsec: " + std::to_string(pow(tan(textAngle), 2)) );
        textLabels[9].setFillColor(sf::Color(255,121,189));
        textLabels[10].setString("Excsc: " + std::to_string(pow(1/tan(textAngle), 2)) );
        textLabels[10].setFillColor(sf::Color(0,100,0));

        // draw update
        window.draw(unitCircle);
        window.draw(yLine, 2, sf::Lines);
        window.draw(xLine, 2, sf::Lines);

        window.draw(mouseLine, 2, sf::Lines);
        window.draw(sinLine1, 2, sf::Lines);
        window.draw(sinLine2, 2, sf::Lines);

        window.draw(cosLine1, 2, sf::Lines);
        window.draw(cosLine2, 2, sf::Lines);

        window.draw(tanLine, 2, sf::Lines);
        window.draw(cotLine, 2, sf::Lines);

        window.draw(secLine, 2, sf::Lines);
        window.draw(cscLine, 2, sf::Lines);

        window.draw(vsnLine, 2, sf::Lines);
        window.draw(cvsLine, 2, sf::Lines);

        window.draw(exsecLine, 2, sf::Lines);
        window.draw(excscLine, 2, sf::Lines);

        for (int i = 0; i < 11; i++) {
            window.draw(textLabels[i]);
        }


        // display draw
        window.display();
    }
}