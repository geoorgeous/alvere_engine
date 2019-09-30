#include "alvere/utils/console.hpp"

#include <sstream>

#include "alvere/graphics/shader.hpp"
#include "alvere/graphics/shader_program.hpp"
#include "alvere/graphics/text/font.hpp"

namespace alvere::console
{
	namespace gui
	{
		bool _initialised = false;
		bool _shown = false;

		Asset<ShaderProgram> _shaderProgram;
		Font::Face _fontFace("res/fonts/consola.ttf");
		unsigned int _lineHeight = 18;
		unsigned int _numLinesToShow = 9;

		AssetRef<Window> _window;

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

		std::vector<std::string> _history;
		std::stringstream _input;

		// draw input box
		// draw history background box
		// draw cursor
		// draw text

		void submitCommand();

		void init(AssetRef<Window> window)
		{
			if (_initialised)
				return;

			Asset<Shader> vShader = Shader::New(Shader::Type::Vertex, R"(
				)");

			Asset<Shader> fShader = Shader::New(Shader::Type::Fragment, R"(
				)");

			_shaderProgram = ShaderProgram::New();
			_shaderProgram->SetShader(AssetRef<Shader>(vShader.get()));
			_shaderProgram->SetShader(AssetRef<Shader>(fShader.get()));
			_shaderProgram->build();

			_window = window;

			_initialised = true;
		}

		void show()
		{
			if (!_initialised)
				return;
		}

		void hide()
		{
			if (!_shown)
				return;
		}

		void update(float deltaTime)
		{
			if (!_shown)
				return;

			if (_window->GetKey(_keyShowHide).justPressed)
			{
				if (_shown)
				{
					hide();
					return;
				}
				else show();
			}

			if (_window->GetKey(_keySubmitCommand).justPressed)
			{
				submitCommand();
			}

			if (_window->GetKey(_keyHistoryBack).justPressed || _window->GetKey(_keyHistoryBack).isRepeating)
			{
				// previous command in history
			}

			if (_window->GetKey(_keyHistoryForward).justPressed || _window->GetKey(_keyHistoryForward).isRepeating)
			{
				// next command in history
			}

			if (_window->GetKey(_keyCaretLeft).justPressed || _window->GetKey(_keyCaretLeft).isRepeating)
			{
				// move caret left
			}

			if (_window->GetKey(_keyCaretRight).justPressed || _window->GetKey(_keyCaretRight).isRepeating)
			{
				// move caret right
			}

			if (_window->GetKey(_keyAutoFill).justPressed || _window->GetKey(_keyAutoFill).isRepeating)
			{
				// display option at caret position
			}

			if (_window->GetKey(_keyAutoFillAccept).justPressed)
			{
				// write the displayed option in to input
			}

			if (_window->GetKey(_keyPageUp).justPressed || _window->GetKey(_keyPageUp).isRepeating)
			{
				// display the previous page of text in the display window
			}

			if (_window->GetKey(_keyPageDown).justPressed || _window->GetKey(_keyPageDown).isRepeating)
			{
				// display the next page of text in the display window
			}
		}

		void draw()
		{
			if (!_shown)
				return;
		}

		void submitCommand()
		{

		}
	}
}