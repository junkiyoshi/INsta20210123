#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	float phi_deg_step = 10;
	float theta_deg_step = 3;
	float threshold = 0.55;

	ofColor color;
	vector<ofColor> base_color_list;
	vector<int> hex_list = { 0xFF9800, 0x03A9F4, 0xf44336 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		base_color_list.push_back(color);
	}
	
	int color_index = 0;
	for (float radius = 100; radius <= 200; radius += 50) {

		auto rotate_noise = glm::vec2(ofRandom(1000), ofRandom(1000));
		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 180; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.025);
				if (noise_value < threshold) { continue; }

				auto noise_1 = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.025);
				auto noise_2 = ofNoise(radius * cos((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02, radius * sin((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.025);
				auto noise_3 = ofNoise(radius * cos((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02, radius * sin((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.02, radius * cos(theta_deg * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.025);
				auto noise_4 = ofNoise(radius * cos(phi_deg * DEG_TO_RAD) * 0.02, radius * sin(phi_deg * DEG_TO_RAD) * 0.02, radius * cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * 0.02, ofGetFrameNum() * 0.025);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

				for (int i = 0; i < vertices.size(); i++) {

					auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(rotate_noise.x, ofGetFrameNum() * 0.001), 0, 1, PI * -2, PI * 2), glm::vec3(0, 1, 0));
					auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(rotate_noise.y, ofGetFrameNum() * 0.001), 0, 1, PI * -2, PI * 2), glm::vec3(1, 0, 0));

					vertices[i] = glm::vec4(vertices[i], 0) * rotation_y * rotation_x;
				}

				this->face.addVertices(vertices);

				for (int i = 0; i < vertices.size(); i++) {

					this->face.addColor(ofColor(base_color_list[color_index]));
				}

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				if (noise_1 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_2 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[2]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_3 < threshold) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_4 < threshold) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}
			}
		}

		color_index++;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->frame.draw();
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}