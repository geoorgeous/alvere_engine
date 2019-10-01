#include "alvere/utils/console.hpp"

#include <sstream>

#include <glad/gl.h>

#include "graphics_api/opengl/opengl_errors.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

#include "alvere/application/window.hpp"
#include "alvere/graphics/buffers.hpp"
#include "alvere/graphics/shader.hpp"
#include "alvere/graphics/shader_program.hpp"
#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/text/font.hpp"
#include "alvere/graphics/texture.hpp"
#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/math/matrix/transformations.hpp"

namespace alvere::console
{
	namespace gui
	{
		bool _initialised = false;
		bool _shown = false;

		Asset<ShaderProgram> _shaderProgram;
		VertexBuffer * _vbo;
		graphics_api::opengl::VertexArray * _vao;
		Font::Face _fontFace("res/fonts/consola.ttf");
		Asset<SpriteBatcher> _spriteBatcher;
		unsigned int _lineHeight = 18;
		unsigned int _numLinesToShow = 9;
		Matrix4 _projection;

		Window * _window;

		Key _keyShowHide = Key::BackQuote;
		Key _keySubmitCommand = Key::Enter;
		Key _keyHistoryBack = Key::Up;
		Key _keyHistoryForward = Key::Down;
		Key _keyCaretLeft = Key::Left;
		Key _keyCaretRight = Key::Right;
		Key _keyAutoFill = Key::Tab;
		Key _keyAutoFillAccept = Key::Space;
		Key _keyPageUp = Key::PageUp;
		Key _keyPageDown = Key::PageDown;

		std::vector<std::string> _commandHistory;
		std::string _input;
		std::stringstream _output;
		unsigned int _caretPosition;

		// draw input box
		// draw history background box
		// draw cursor
		// draw text

		void submitCommand();

		void init(Window * window)
		{
			if (_initialised)
				return;

			Asset<Shader> vShader = Shader::New(Shader::Type::Vertex, R"(#version 330 core
					uniform mat4 u_projectionMatrix;

					layout(location = 0) in vec3 a_position;

					void main()
					{
						gl_Position = u_projectionMatrix * vec4(a_position, 1.0f);
					}
				)");

			Asset<Shader> fShader = Shader::New(Shader::Type::Fragment, R"(#version 330 core
					uniform vec3 u_colour;
					uniform int u_lineHeight;
					uniform int u_lineCount;

					out vec4 f_colour;

					void main()
					{
						int y = int(gl_FragCoord.y);

						if (y > u_lineHeight * (u_lineCount + 1))
							discard;

						if (y <= u_lineHeight)
							f_colour = vec4(u_colour, 0.5f);
						else
							f_colour = vec4(u_colour, 0.8f);
					}
				)");

			_shaderProgram = ShaderProgram::New();
			_shaderProgram->SetShader(AssetRef<Shader>(vShader.get()));
			_shaderProgram->SetShader(AssetRef<Shader>(fShader.get()));
			_shaderProgram->build();

			_window = window;

			unsigned int width = _window->getWidth();
			unsigned int height = _window->getHeight();

			_projection = orthographic(0, width, height, 0, -1, 1);

			_shaderProgram->bind();
			_shaderProgram->sendUniformMat4x4("u_projectionMatrix", _projection);
			_shaderProgram->sendUniformFloat3("u_colour", 0.2f, 0.2f, 0.2f);
			_shaderProgram->sendUniformInt1("u_lineHeight", _fontFace.getBitmap(18)->getFontFaceHeight());
			_shaderProgram->sendUniformInt1("u_lineCount", 9);

			float vertexData[18] = {
				0.0f, height, 0.0f,
				width, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,

				0.0f, height, 0.0f,
				width, height, 0.0f,
				width, 0.0f, 0.0f,
			};

			_vbo = VertexBuffer::New(vertexData, sizeof(vertexData));
			_vbo->SetLayout(BufferLayout({
				BufferElementProperties(Shader::DataType::Float3, "a_position", false)
				}));

			_vao = new graphics_api::opengl::VertexArray;
			_vao->AddVertexBuffer(_vbo);

			_spriteBatcher = SpriteBatcher::New();

			_initialised = true;
		}

		void destroy()
		{
			delete _vbo;
			delete _vao;

			_initialised = false;
		}

		void show()
		{
			if (!_initialised)
				return;

			_shown = true;
		}

		void hide()
		{
			if (!_shown)
				return;

			_shown = false;
		}

		void update(float deltaTime)
		{
			if (_window->getKey(_keyShowHide).justPressed)
			{
				if (_shown)
				{
					hide();
					return;
				}
				else show();
			}

			if (!_shown)
				return;

			if (_window->getKey(_keySubmitCommand).justPressed)
			{
				submitCommand();
			}

			if (_window->getKey(_keyHistoryBack).justPressed || _window->getKey(_keyHistoryBack).isRepeating)
			{
				// previous command in history
			}

			if (_window->getKey(_keyHistoryForward).justPressed || _window->getKey(_keyHistoryForward).isRepeating)
			{
				// next command in history
			}

			if (_window->getKey(_keyCaretLeft).justPressed || _window->getKey(_keyCaretLeft).isRepeating)
			{
				// move caret left
			}

			if (_window->getKey(_keyCaretRight).justPressed || _window->getKey(_keyCaretRight).isRepeating)
			{
				// move caret right
			}

			if (_window->getKey(_keyAutoFill).justPressed || _window->getKey(_keyAutoFill).isRepeating)
			{
				// display option at caret position
			}

			if (_window->getKey(_keyAutoFillAccept).justPressed)
			{
				// write the displayed option in to input
			}

			if (_window->getKey(_keyPageUp).justPressed || _window->getKey(_keyPageUp).isRepeating)
			{
				// display the previous page of text in the display window
			}

			if (_window->getKey(_keyPageDown).justPressed || _window->getKey(_keyPageDown).isRepeating)
			{
				// display the next page of text in the display window
			}
		}

		void draw()
		{
			if (!_shown)
				return;

			glClear(GL_DEPTH_BUFFER_BIT);

			_shaderProgram->bind();

			_vao->Bind();
			ALV_LOG_OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, 18));

			_spriteBatcher->begin(_projection);
			_spriteBatcher->submit(*_fontFace.getBitmap(18), "hello world", Vector2(0.0f, 0.0f));
			_spriteBatcher->end();
		}

		void submitCommand()
		{

		}
	}
}