#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    float width = ofGetWidth() * .12;
    float height = ofGetHeight() * .12;
    colorAnimationSpeed = 0.1;
    rotationX = 0;
    rotationY = 0;
    pause = false;

    ofIcoSpherePrimitive icoSphere = ofIcoSpherePrimitive(250, 3);
    // model.loadModel("dna.dae");
    // model.setScale(5, 5, 5);
    baseMesh = icoSphere.getMesh();
    // baseMesh = model.getMesh(0);

    ofEnableDepthTest();
    baseMesh.enableColors();
    baseMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);
    savePDF = false;
    int numVerts = baseMesh.getNumVertices();
    colorPalette = {
        ofColor(114, 197, 236), 
        ofColor(222, 69, 146), 
        ofColor(10, 10, 11), 
    };
    for (int i = 0; i < numVerts; i++)
    {
        ofVec3f vertex = baseMesh.getVertex(i);
        ofVec3f offset = offsets[i];

        // only to scale a 3d model via vertex scale
        // vertex.x = vertex.x * 300.0;
        // vertex.y = vertex.y * 300.0;
        // vertex.z = vertex.z * 300.0;
        // baseMesh.setVertex(i, vertex);

        offsets.push_back(ofVec3f(ofRandom(0, 100000), ofRandom(0, 100000), ofRandom(0, 100000)));
        ofColor color(255);
        // baseMesh.addColor(color);
        baseMesh.addColor(colorPalette[i % colorPalette.size()]);
    }
    for (int i = 0; i < 1000; i++)
    {
        stars.push_back(ofVec3f(ofRandom(-1000, 1000), ofRandom(-1000, 1000), ofRandom(-1000, 1000)));
        starColors.push_back(colorPalette[i % colorPalette.size()]);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (pause)
    {
        return;
    }
    rotationX += 0.1;
    rotationY += 0.05;
    int numVerts = baseMesh.getNumVertices();
    float time = ofGetElapsedTimef();
    float timeScale = 2.0;
    float displacementScale = 0.5;

    for (int i = 0; i < numVerts; i++)
    {
        ofVec3f vertex = baseMesh.getVertex(i);
        ofVec3f offset = offsets[i];

        vertex.x += (ofSignedNoise(time * timeScale + offset.x)) * displacementScale;
        vertex.y += (ofSignedNoise(time * timeScale + offset.y)) * displacementScale;
        vertex.z += (ofSignedNoise(time * timeScale + offset.z)) * displacementScale;
        baseMesh.setVertex(i, vertex);

        float offsetX = vertex.x * 0.01;
        float offsetY = vertex.y * 0.01;
        float offsetZ = vertex.z * 0.01;

        // Use noise to create smooth transitions between colors
        float noiseValue = ofNoise(offsetX, offsetY, offsetZ, time * colorAnimationSpeed);

        // Map the noise value to an index in the color palette
        int colorIndex = ofMap(noiseValue, 0, 1, 0, colorPalette.size() - 1);
        int nextColorIndex = (colorIndex + 1) % colorPalette.size();

        // Interpolate between two colors
        float lerpAmount = fmod(ofMap(noiseValue, 0, 1, 0, colorPalette.size()), 1.0);
        ofColor interpolatedColor = colorPalette[colorIndex].getLerped(colorPalette[nextColorIndex], lerpAmount);

        // Apply the color to the vertex
        // baseMesh.setColor(i, interpolatedColor);
        // baseMesh.setColor(i, color);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // ofBackgroundGradient(ofColor(20, 20, 20), ofColor(5, 5, 30), OF_GRADIENT_CIRCULAR);
    ofColor baseColor(255, 234, 98);
    ofColor complementaryColor = baseColor.getInverted();
    ofColor endColor = baseColor.getLerped(complementaryColor, 0.1);
    ofBackgroundGradient(baseColor, endColor, OF_GRADIENT_LINEAR);

    cam.begin();
    if (!pause)
    {
        ofRotateXDeg(rotationX);
        ofRotateYDeg(rotationY);
    }

    // Draw stars
    ofPushStyle();
    ofSetColor(255);
    for (int i = 0; i < stars.size(); i++)
    {
        ofSetColor(starColors[i]);
        ofDrawSphere(stars[i], 1);
    }
    ofPopStyle();

    if (savePDF)
    {
        ofBeginSaveScreenAsPDF("./render/screenshot_" + ofGetTimestampString() + ".pdf");
    }
    ofPushMatrix();
    glPointSize(4);
    if (!pause)
    {
        ofRotateYDeg(ofGetElapsedTimef() * 20);
    }
    baseMesh.draw();
    // model.drawWireframe();
    ofPopMatrix();
    if (savePDF)
    {
        ofEndSaveScreenAsPDF();
        savePDF = false;
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case ' ':
        cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
        break;
    case 'F':
    case 'f':
        ofToggleFullscreen();
        break;
    case 'p':
        savePDF = true;
    case 's':
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
        break;
    case 'a':
        pause = !pause;
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
