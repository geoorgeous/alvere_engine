#include "alvere/utils/console.hpp"

#include <algorithm>
#include <memory>
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
#include "alvere/utils/utilities.hpp"

namespace alvere::console
{
	void registerCommand(const Command & command);

	void unregisterCommand(const Command & command);

	Command::IParam::IParam(const std::string & name, const std::string & description, bool isRequired, std::type_index typeIndex, const char * typeString)
		: m_name(name), m_description(description), m_isRequired(isRequired), m_typeIndex(typeIndex)
	{
		m_detailedName = isRequired ? "<()>" : "[()]";
		m_detailedName.insert(2, typeString);
		m_detailedName.insert(1, m_name);
	}

	Command::IArg::IArg(std::type_index typeIndex)
		: m_typeIndex(typeIndex)
	{ }

	Command::Command(const char * name, const char * description, std::vector<IParam *> params, Function f)
		: m_name(name), m_description(description), m_f(f)
	{
		m_signature = m_name;

		m_params.resize(params.size());
		for (int i = 0; i < params.size(); ++i)
		{
			m_params[i] = params[i]->clone();
			m_signature += " " + m_params[i]->getDetailedName();
		}

		registerCommand(*this);
	}

	Command::~Command()
	{
		unregisterCommand(*this);

		for (int i = 0; i < m_params.size(); ++i)
			delete m_params[i];
	}

	bool Command::tryInvoke(const std::vector<std::string> & argStrings, std::string & output) const
	{
		if (argStrings.size() > m_params.size())
		{
			output = "Too many arguments supplied.\nType `help " + m_name + "` to see the expected parameters.";
			return false;
		}
		else if (argStrings.empty() && m_params.empty())
		{
			output = m_f({});
			return true;
		}

		std::vector<const IArg *> args(m_params.size(), nullptr);

		size_t currentArgString = 0;

		bool parseFailure = false;

		for (size_t p = 0; p < m_params.size(); ++p)
		{
			if (currentArgString == argStrings.size())
			{
				if (m_params[p]->getIsRequired())
				{
					output = "Missing argument for required parameter " + m_params[p]->getDetailedName() + ".\nType `help " + m_name + "` to see the expected parameters.";
					parseFailure = true;
					break;
				}
				else
					continue;
			}

			args[currentArgString] = m_params[p]->tryParseStringToArg(argStrings[currentArgString], output);

			if (args[currentArgString] != nullptr)
			{
				++currentArgString;
			}
			else if (m_params[p]->getIsRequired())
			{
				output = "Failed to parse argument " + std::to_string(currentArgString) + " (" + argStrings[currentArgString] + ") for required parameter " + m_params[p]->getDetailedName() + ((output == "") ? "" : ": " + output);
				parseFailure = true;
				break;
			}
		}

		if (!parseFailure && currentArgString < argStrings.size())
		{
			output = "Too many arguments provided.\nType `help " + m_name + "` to see the expected parameters.";
			parseFailure = true;
		}

		if (!parseFailure)
		{
			output = m_f(args);
		}

		for (const IArg * arg : args)
			delete arg;

		return parseFailure;
		return false;
	}

	std::vector<const Command *> _commands;

	namespace gui
	{
		bool _initialised = false;
		bool _shown = false;

		Asset<ShaderProgram> _shaderProgram;
		VertexBuffer * _vbo;
		graphics_api::opengl::VertexArray * _vao;
		Matrix4 _projection;

		Asset<SpriteBatcher> _spriteBatcher;
		Font::Face _fontFace("res/fonts/consola.ttf");
		const Font::Face::Bitmap * _fontFaceBitmap;
		unsigned int _fontSize = 14;
		unsigned int _maxOutputLineCountShrunk = 14;
		unsigned int _maxOutputLineCountExpanded = 48;
		unsigned int _maxOutputLineCount = _maxOutputLineCountShrunk;

		Window * _window;
		CharInputEvent::Handler _charInputEventHandler;

		std::vector<std::string> _output;
		unsigned int _outputPageIndex = 0;
		std::vector<std::string> _inputHistory;
		std::string _inputPre = "> ";
		std::string _input;
		std::string _inputSuggestion;
		std::vector<std::string> _suggestions;
		int _currentHistory = -1;
		unsigned int _caretPosition;
		bool _caretIsVisible = true;
		float _caretFlashSpeed = 0.5f;
		float _timeUntilCaretToggle = _caretFlashSpeed;

		Key _keyShowHide = Key::BackQuote;
		Key _keySubmitCommand = Key::Enter;
		Key _keyHistoryBack = Key::Up;
		Key _keyHistoryForward = Key::Down;
		Key _keyCaretLeft = Key::Left;
		Key _keyCaretRight = Key::Right;
		Key _keyEraseCharLeft = Key::BackSpace;
		Key _keyEraseCharRight = Key::Delete;
		Key _keyApplySuggestion = Key::Tab;
		Key _keyAcceptSuggestion = Key::Space;
		Key _keyPageUp = Key::PageUp;
		Key _keyPageDown = Key::PageDown;

		std::vector<std::unique_ptr<Command>> _builtInCommands;

		void submitInput();
		void viewHistoryBack();
		void viewHistoryForward();
		int getLeftSkipIndex();
		int getRightSkipIndex();
		void moveCaretLeft(bool skipWord = false);
		void moveCaretRight(bool skipWord = false);
		void eraseCharLeft(bool skipWord = false);
		void eraseCharRight(bool skipWord = false);
		void tryCycleSuggestions();
		void tryApplySuggestion();
		void pageUpOutput();
		void pageDownOutput();
		void onCharInput(unsigned int);
		void updateInputSuggestion();
		void clearInput();
		void resetCaretVisibility();

		void init(Window * window)
		{
			if (_initialised)
				return;

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"help", 
				"Displays a list of all of the available commands, and can optionally be used to display further information about a single command.", 
				std::vector<Command::IParam *> {
					&Command::StringParam("command name", "The name of the command to display information about.", false) },
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					std::string output;

					if (args[0] == nullptr)
					{
						output = "Below is a list of all of the available commands. To see more information about a particular command, type 'help' with the name of the command.\n\nSyntax:\n[optional parameter]\n<required parameter>\n(parameter type)\n(enum|values)\n\n";

						for (const Command * command : _commands)
							output += command->getSignature() + " : " + command->getDescription() + "\n";
					}
					else
					{
						bool commandFound = false;

						const std::string & commandName = args[0]->getValue<std::string>();

						for (const Command * command : _commands)
						{
							if (command->getName() == commandName)
							{
								commandFound = true;

								output += command->getSignature() + "\n\t";
								output += command->getDescription() + "\n\t";

								const std::vector<Command::IParam *> & params = command->getParams();

								for (const Command::IParam * param : params)
									output += param->getDetailedName() + " : " + param->getDescription() + "\n\t";
							}
						}

						if (!commandFound)
							output = "Command '" + commandName + "' not found. Type 'help' for a list of available commands.";
					}

					return output;
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"console.clear", 
				"Clears the console output.", 
				std::vector<Command::IParam *> {},
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					_output.clear();
					return "";
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"console.expand", 
				"Expands the console window.", 
				std::vector<Command::IParam *> {},
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					_maxOutputLineCount = _maxOutputLineCountExpanded;
					_shaderProgram->bind();
					_shaderProgram->sendUniformInt1("u_outputLineCount", _maxOutputLineCount);
					return "";
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"console.shrink", 
				"Shrinks the console window.", 
				std::vector<Command::IParam *> {},
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					_maxOutputLineCount = _maxOutputLineCountShrunk;
					_shaderProgram->bind();
					_shaderProgram->sendUniformInt1("u_outputLineCount", _maxOutputLineCount);
					return "";
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"alias.new",
				"Creates an alias for some console input.",
				std::vector<Command::IParam *> {
					&Command::StringParam("new alias name", "The name of the alias being created.", true),
					& Command::StringParam("command string", "The command which will be executed when the new alias is called.", true),
					& Command::StringParam("description", "A description of the new alias", false), },
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					return "NOT IMPLEMENTED!";
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"alias.delete",
				"Deletes an existing alias.",
				std::vector<Command::IParam *> {
					&Command::StringParam("alias name", "The name of the alias to delete.", true), },
				[&](std::vector<const Command::IArg *> args) -> std::string
				{
					return "NOT IMPLEMENTED!";
				}));

			_builtInCommands.emplace_back(std::make_unique<Command>(
				"test",
				"This is a test command.",
				std::vector<Command::IParam *> {
					&Command::BoolParam("pbool", "test pool param", true),
					&Command::UIntParam("puint", "test unsigned int param", true),
					&Command::IntParam("pint", "test int param", true),
					&Command::FloatParam("pfloat", "test float param", true),
					&Command::StringParam("pstring", "test string param", true),
					&Command::EnumParam("penum", "test enum param", true, { "one", "two", "three" }), },
				[](std::vector<const Command::IArg *> args) -> std::string
				{
					return "test command";
				}));

			_fontFaceBitmap = _fontFace.getBitmap(_fontSize);

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
					uniform int u_outputLineCount;

					out vec4 f_colour;

					void main()
					{
						int y = int(gl_FragCoord.y);

						if (y > u_lineHeight * (u_outputLineCount + 1))
							discard;

						if (y <= u_lineHeight)
							f_colour = vec4(u_colour, 0.925f);
						else
							f_colour = vec4(u_colour, 0.8f);
					}
				)");

			_shaderProgram = ShaderProgram::New();
			_shaderProgram->SetShader(AssetRef<Shader>(vShader.get()));
			_shaderProgram->SetShader(AssetRef<Shader>(fShader.get()));
			_shaderProgram->build();

			_window = window;
			_charInputEventHandler.setFunction(onCharInput);
			_window->getEvent<CharInputEvent>()->subscribe(_charInputEventHandler);

			unsigned int width = _window->getWidth();
			unsigned int height = _window->getHeight();

			_projection = orthographic(0, width, height, 0, -1, 1);

			_shaderProgram->bind();
			_shaderProgram->sendUniformMat4x4("u_projectionMatrix", _projection);
			_shaderProgram->sendUniformFloat3("u_colour", 0.0f, 0.0f, 0.0f);
			_shaderProgram->sendUniformInt1("u_lineHeight", _fontFaceBitmap->getFontFaceHeight());
			_shaderProgram->sendUniformInt1("u_outputLineCount", _maxOutputLineCount);

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

			_output.emplace_back("Type 'help' for a list of available commands.");
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

			clearInput();

			_outputPageIndex = 0;
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

			if ((_timeUntilCaretToggle -= deltaTime) <= 0.0f)
			{
				_timeUntilCaretToggle = _caretFlashSpeed;
				_caretIsVisible = !_caretIsVisible;
			}

			if (_window->getKey(_keySubmitCommand).justPressed)
				submitInput();
			else if (_window->getKey(_keyHistoryBack).justPressed || _window->getKey(_keyHistoryBack).isRepeating)
				viewHistoryBack();
			else if (_window->getKey(_keyHistoryForward).justPressed || _window->getKey(_keyHistoryForward).isRepeating)
				viewHistoryForward();
			else if (_window->getKey(_keyCaretLeft).justPressed || _window->getKey(_keyCaretLeft).isRepeating)
				moveCaretLeft(_window->getKey(Key::LeftCtrl).isDown || _window->getKey(Key::RightCtrl).isDown);
			else if (_window->getKey(_keyCaretRight).justPressed || _window->getKey(_keyCaretRight).isRepeating)
				moveCaretRight(_window->getKey(Key::LeftCtrl).isDown || _window->getKey(Key::RightCtrl).isDown);
			else if (_window->getKey(_keyEraseCharLeft).justPressed || _window->getKey(_keyEraseCharLeft).isRepeating)
				eraseCharLeft(_window->getKey(Key::LeftCtrl).isDown || _window->getKey(Key::RightCtrl).isDown);
			else if (_window->getKey(_keyEraseCharRight).justPressed || _window->getKey(_keyEraseCharRight).isRepeating)
				eraseCharRight(_window->getKey(Key::LeftCtrl).isDown || _window->getKey(Key::RightCtrl).isDown);
			else if (_window->getKey(_keyApplySuggestion).justPressed || _window->getKey(_keyApplySuggestion).isRepeating)
				tryCycleSuggestions();
			else if (_window->getKey(_keyAcceptSuggestion).justPressed)
				tryApplySuggestion();
			else if (_window->getKey(_keyPageUp).justPressed || _window->getKey(_keyPageUp).isRepeating)
				pageUpOutput();
			else if (_window->getKey(_keyPageDown).justPressed || _window->getKey(_keyPageDown).isRepeating)
				pageDownOutput();
		}

		void draw()
		{
			if (!_shown)
				return;

			float inputXOffset = _fontFaceBitmap->getTextSize(_inputPre).x;
			float caretXOffset = _fontFaceBitmap->getTextSize(_input.substr(0, _caretPosition)).x;

			_spriteBatcher->begin(_projection);

			_spriteBatcher->submit(*_fontFaceBitmap, _inputPre.c_str(), Vector2(3.0f, 6.0f));

			_spriteBatcher->submit(*_fontFaceBitmap, _input.c_str(), Vector2(3.0f + inputXOffset, 6.0f));

			if (_caretIsVisible)
				_spriteBatcher->submit(*_fontFaceBitmap, "_", Vector2(3.0f + inputXOffset + caretXOffset, 6.0f));

			int lineCount = 0;
			for (int i = _outputPageIndex * _maxOutputLineCount; i < _output.size() && lineCount < _maxOutputLineCount; ++i, ++lineCount)
			{
				_spriteBatcher->submit(*_fontFaceBitmap, _output[_output.size() - 1 - i], Vector2(3.0f, 6.0f + _fontFaceBitmap->getFontFaceHeight() * (lineCount + 1)));
			}

			_shaderProgram->bind();
			_shaderProgram->sendUniformInt1("u_outputLineCount", lineCount);
			_vao->Bind();
			ALV_LOG_OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, 18));

			_spriteBatcher->end();
		}

		void submitInput()
		{
			_output.emplace_back("");
			_output.emplace_back(_inputPre + _input);
			_inputHistory.insert(_inputHistory.begin(), _input);

			std::string output = submitCommand(_input);

			if (!output.empty())
			{
				std::vector<std::string> lines = utils::splitString(output, '\n', true);
				_output.insert(_output.end(), lines.begin(), lines.end());
			}

			resetCaretVisibility();

			clearInput();
		}

		void viewHistoryBack()
		{
			if (_currentHistory == _inputHistory.size() - 1)
				return;

			++_currentHistory;

			_input = _inputHistory[_currentHistory];
			_caretPosition = _input.size();
			resetCaretVisibility();
		}

		int getLeftSkipIndex()
		{
			bool wasOnChar = false;
			size_t pos = _caretPosition;
			while (pos > 0)
			{
				if (wasOnChar && std::isspace(_input[pos - 1]))
					break;
				--pos;
				wasOnChar = !std::isspace(_input[pos]);
			}
			return pos;
		}

		int getRightSkipIndex()
		{
			bool wasOnSpace = false;
			int pos = _caretPosition;
			while (pos < _input.length())
			{
				if (wasOnSpace && !std::isspace(_input[pos]))
					break;
				wasOnSpace = std::isspace(_input[pos]);
				++pos;
			}
			return pos;
		}

		void viewHistoryForward()
		{
			if (_currentHistory == -1)
				return;

			if (_currentHistory == 0)
			{
				clearInput();
				return;
			}

			--_currentHistory;

			_input = _inputHistory[_currentHistory];
			_caretPosition = _input.size();
			resetCaretVisibility();
		}

		void moveCaretLeft(bool skipWord)
		{
			resetCaretVisibility();

			if (skipWord)
			{
				_caretPosition = getLeftSkipIndex();
			}
			else if (_caretPosition > 0)
			{
				--_caretPosition;
			}
		}

		void moveCaretRight(bool skipWord)
		{
			resetCaretVisibility();

			if (skipWord)
			{
				_caretPosition = getRightSkipIndex();
			}
			else if (_caretPosition < _input.length())
			{
				++_caretPosition;
			}
		}

		void eraseCharLeft(bool skipWord)
		{
			resetCaretVisibility();

			if (_caretPosition == 0)
				return;

			size_t caretOrigin = _caretPosition;

			moveCaretLeft(skipWord);

			_input.erase(_input.begin() + _caretPosition, _input.begin() + caretOrigin);
		}

		void eraseCharRight(bool skipWord)
		{
			resetCaretVisibility();

			if (_caretPosition == _input.size())
				return;

			auto eraseLast = skipWord ? _input.begin() + getRightSkipIndex() : _input.begin() + _caretPosition + 1;

			_input.erase(_input.begin() + _caretPosition, eraseLast);
		}

		void tryCycleSuggestions()
		{
			// todo: pressing tab will try and display the next available suggestion
		}

		void tryApplySuggestion()
		{
			// todo: pressing space when a suggestion is displayed will apply the suggestion to the input
		}

		void pageUpOutput()
		{
			if ((unsigned int)_output.size() - (_outputPageIndex * _maxOutputLineCount) > _maxOutputLineCount)
				++_outputPageIndex;
		}

		void pageDownOutput()
		{
			if (_outputPageIndex == 0)
				return;

			--_outputPageIndex;
		}

		void onCharInput(unsigned int utfCodePoint)
		{
			if (!_shown)
				return;

			resetCaretVisibility();

			_input.insert(_input.begin() + _caretPosition, (char)utfCodePoint);

			moveCaretRight();

			updateInputSuggestion();
		}

		void updateInputSuggestion()
		{
			if (_currentHistory > -1)
			{
				_inputSuggestion = _inputHistory[_currentHistory];
				return;
			}

			_suggestions.clear();

			// todo: try and find suggestions for command names and paameters

			if (_suggestions.size() > 0)
			{
				// todo: set suggestion thing
			}
		}

		void clearInput()
		{
			_input = "";
			_caretPosition = 0;
			_currentHistory = -1;

			resetCaretVisibility();
		}

		void resetCaretVisibility()
		{
			_caretIsVisible = true;
			_timeUntilCaretToggle = _caretFlashSpeed;
		}
	}

	std::string submitCommand(const std::string & command)
	{
		std::vector<std::string> parts;
		size_t partBegin;
		char delim(0);

		for (size_t i = 0; i < command.length(); ++i)
		{
			if (std::isspace(command[i]))
				continue;
			
			delim = 0;
			partBegin = i;

			if (command[i] == '"' || command[i] == '\'')
			{
				++partBegin;

				if (partBegin >= command.length())
				{
					return (std::string("Expecting matching delimiter (") + command[i]) + ")";
				}
						
				delim = command[i];
			}

			for (size_t j = partBegin; ; ++j)
			{
				if (delim)
				{
					if (j >= command.length())
					{
						return (std::string("Expecting matching delimiter (") + command[i]) + ")";
					}
					else if (command[j] == delim)
					{
						parts.emplace_back(command.substr(partBegin, j - partBegin));
						i = j;
						break;
					}
				}
				else if (j == command.length() || isspace(command[j]))
				{
					parts.emplace_back(command.substr(partBegin, j - partBegin));
					i = j;
					break;
				}
			}
		}

		if (parts.size() == 0)
			return "";

		const std::string & commandName = parts[0];
		std::vector<std::string> args(parts.begin() + 1, parts.begin() + parts.size());

		std::string output;

		bool commandFound = false;

		for (const Command * command : _commands)
		{
			if (command->getName() == commandName)
			{
				commandFound = true;

				if (command->tryInvoke(args, output))
				{
					return output;
				}
			}
		}

		if (commandFound)
		{
			return output;
		}

		return "Command '" + commandName + "' not found. Type 'help' for a list of available commands.";
	}

	void registerCommand(const Command & command)
	{
		auto iter = std::find(_commands.begin(), _commands.end(), &command);

		if (iter != _commands.end())
			return;

		_commands.emplace_back(&command);

		std::sort(_commands.begin(), _commands.end(), [](const Command * a, const Command * b) -> bool { return a->getName() < b->getName(); });
	}

	void unregisterCommand(const Command & command)
	{
		auto iter = std::find(_commands.begin(), _commands.end(), &command);

		if (iter == _commands.end())
			return;

		_commands.erase(iter);
	}
}