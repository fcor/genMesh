#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // ofBackground(39);
    float width = ofGetWidth() * .12;
    float height = ofGetHeight() * .12;

    rotationX = 0;
    rotationY = 0;

    ofIcoSpherePrimitive icoSphere = ofIcoSpherePrimitive(250, 3);
    baseMesh = icoSphere.getMesh();
    ofEnableDepthTest();
    baseMesh.enableColors();
    baseMesh.setMode(OF_PRIMITIVE_POINTS);
    savePDF = false;
    int numVerts = baseMesh.getNumVertices();
    for (int i = 0; i < numVerts; i++)
    {
        offsets.push_back(ofVec3f(ofRandom(0, 100000), ofRandom(0, 100000), ofRandom(0, 100000)));
        ofColor color(255);
        baseMesh.addColor(color);
    }
    for (int i = 0; i < 1000; i++)
    {
        stars.push_back(ofVec3f(ofRandom(-1000, 1000), ofRandom(-1000, 1000), ofRandom(-1000, 1000)));
        starColors.push_back(ofColor(ofRandom(100, 255)));
    }
}

//--------------------------------------------------------------
void ofApp::update()
{   
    rotationX += 0.1;
    rotationY += 0.05;
    int numVerts = baseMesh.getNumVertices();
    float time = ofGetElapsedTimef();
    float timeScale = 2.0;
    float displacementScale = 1.2;
    
    for (int i = 0; i < numVerts; i++)
    {
        ofVec3f vertex = baseMesh.getVertex(i);
        ofVec3f offset = offsets[i];

        vertex.x += (ofSignedNoise(time * timeScale + offset.x)) * displacementScale;
        vertex.y += (ofSignedNoise(time * timeScale + offset.y)) * displacementScale;
        vertex.z += (ofSignedNoise(time * timeScale + offset.z)) * displacementScale;
        baseMesh.setVertex(i, vertex);

        float noiseValue = ofNoise(vertex.x * 0.01, vertex.y * 0.01, vertex.z * 0.01, time * 0.1);

        float r = ofMap(sin(time + vertex.x * 0.01), -1, 1, 0, 255);
        float g = ofMap(cos(time + vertex.y * 0.01), -1, 1, 0, 255);
        float b = ofMap(noiseValue, 0, 1, 0, 255);
        ofColor color(r, g, b);
        baseMesh.setColor(i, color);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackgroundGradient(ofColor(20, 20, 20), ofColor(5, 5, 30), OF_GRADIENT_CIRCULAR);
    
    cam.begin();
    ofRotateXDeg(rotationX);
    ofRotateYDeg(rotationY);

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
    ofRotateYDeg(ofGetElapsedTimef() * 20);
    baseMesh.draw();
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
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".jpg");
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
