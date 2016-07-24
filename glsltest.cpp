/*
 * =====================================================================================
 *
 *       Filename:  glsltest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月24日 19時53分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

GLuint vert_shader;
GLuint frag_shader;

GLuint program_handle;

GLuint vao_handle;

glm::mat4 projection;

void setMatrix(void)
{
	//回転する角度
	static int degree = 0;
	//ビュー変換行列の設定
	glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 5.0f),//カメラの位置
			glm::vec3(0.0f, 0.0f, 0.0f),//視点の位置
			glm::vec3(0.0f, 1.0f, 0.0f) //カメラの上方向の向き
			);
	//モデル変換行列の設定
	//y軸周りに回転させる行列
	float d = 3.141592f * static_cast<float>(degree) / 180.0f;
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), d, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//モデル・ビュー・プロジェクション行列を設定
	glm::mat4 mvp = projection * view * model;
	
	//角度の更新
	degree += 1;
	degree = (degree >= 360) ? (degree - 360) : degree;
	
	GLuint location = glGetUniformLocation(program_handle, "rotationMatrix");
	
	if(location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
	}
}



void display(void)
{
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	glBindVertexArray(vao_handle);
	glDrawArrays(GL_TRIANGLES, 0,3);
	
	
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}

void idle(void)
{
	glutPostRedisplay();
}

void resize(int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);
	
	//プロジェクション行列の設定
	projection = glm::perspective( 3.141592f * 30.0f / 180.0f, static_cast<float>(w) / static_cast<float>(h), 1.0f, 1000.f);
}

void initCallbacks(void)
{
	//画面更新関数の設定
	glutDisplayFunc(display);
	//アイドル時に実行される関数の設定
	glutIdleFunc(idle);
	//画面リサイズ時に実行される関数の設定
	glutReshapeFunc(resize);
}


std::string readShaderFile(const std::string& filename)
{
	std::ifstream f(filename);
	std::string s{};
	std::stringstream ss{};
	while(std::getline(f, s))
	{
		ss << s << std::endl;
	}
	return ss.str();
}

void initShader(void)
{
	//バーテックスシェーダの生成
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	if(vert_shader == 0)
	{
		std::cout << "error at glCreateShaderGL_VERTEX_SHADER()." << std::endl;
	}
	
	//フラグメントシェーダの生成
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if(frag_shader == 0)
	{
		std::cout << "error at glCreateShader(GL_FRAGMENT_SHADER)." << std::endl;
	}
	
	//バーテックスシェーダのコードの読み込み
	std::string vert_shader_code = readShaderFile("basic.vert");
	//フラグメントシェーダのコードの読み込み
	std::string frag_shader_code = readShaderFile("basic.frag");
	//シェーダのコードを配列に格納する
	const GLchar* vert_code_array[] = {vert_shader_code.c_str()};
	const GLchar* frag_code_array[] = {frag_shader_code.c_str()};
	//シェーダへのソースの読み込み
	glShaderSource(vert_shader, 1, vert_code_array, NULL);
	glShaderSource(frag_shader, 1, frag_code_array, NULL);
	
	GLint result;
	
	//バーテックスシェーダのコンパイル
	glCompileShader(vert_shader);
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		std::cout << "error at glGetShaderiv(vert_shader,*)." << std::endl;
	}
	
	
	//フラグメントシェーダのコンパイル
	glCompileShader(frag_shader);
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		std::cout << "error at glGetShaderiv(frag_shader,*)." << std::endl;
		GLint log_len;
		glGetShaderiv(frag_shader,GL_INFO_LOG_LENGTH, &log_len);
		if(log_len > 0)
		{
			std::unique_ptr<char> log(new char[log_len]);
			GLsizei written;
			glGetShaderInfoLog(frag_shader, log_len, &written, log.get());
			std::cout << log.get() << std::endl;
		}
	}
	
}

void initShaderProgram(void)
{
	//シェーダプログラムの初期化
	program_handle = glCreateProgram();
	if(program_handle == 0)
	{
		std::cout << "error at glCreateProgram()." << std::endl;
	}
	//コンパイルしたシェーダプログラムの割り当て
	glAttachShader(program_handle, vert_shader);
	glAttachShader(program_handle, frag_shader);
	
	//シェーダプログラムのリンク
	glLinkProgram(program_handle);
	
	GLint status;
	
	
	//リンクの結果確認
	glGetProgramiv(program_handle, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cout << "error at glLinkProgram(program_handle)." << std::endl;
	}
	else
	{
		//エラーがなければOpenGLパイプラインにインストール
		glUseProgram(program_handle);
	}
}

void initScene(void)
{
	initShader();
	initShaderProgram();
	
	projection = glm::perspective(3.141592f * 30.0f / 180.0f, 1.0f / 1.0f, 1.0f, 1000.f);
	
	//位置データ
	float position_data[] = {
			-0.8f, -0.8f, 0.0f,
			 0.8f, -0.8f, 0.0f,
			 0.0f,  0.8f, 0.0f
			};
	//色データ
	float color_data[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
			};
	//バッファオブジェクトの生成
	GLuint vbo_handles[2];
	glGenBuffers(2, vbo_handles);
	GLuint position_buffer_handle = vbo_handles[0];
	GLuint color_buffer_handle = vbo_handles[1];
	
	//バッファへの位置データの格納
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_handle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), position_data, GL_STATIC_DRAW);
	//バッファへの色データの格納
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_handle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), color_data, GL_STATIC_DRAW);
	
	
	//VAOの生成
	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);
	
	//バーテックスシェーダのインプットの設定を有効にする
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	//インデックス0を位置データに割り当てる
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer_handle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	
	//インデックス1を色データに割り当てる
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer_handle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	
	
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	//ディスプレイモードの設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//ウィンドウの生成
	glutCreateWindow("glsltest");
	//glewの初期化
	GLenum err;
	err = glewInit();
	if(err != GLEW_OK)
	{
		std::cout << "error at glewInit()." << std::endl;
		return 1;
	}
	//コールバック関数の設定
	initCallbacks();
	//シーンの初期化
	initScene();
	//メインループ
	glutMainLoop();
	
	return 0;
}




