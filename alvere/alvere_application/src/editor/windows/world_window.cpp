#include <string>

#include <alvere/application/window.hpp>
#include <alvere/world/world.hpp>

#include "imgui/imgui.h"
#include "editor/imgui_editor.hpp"
#include "editor/windows/world_window.hpp"
#include "components/c_name.hpp"

WorldWindow::WorldWindow(ImGuiEditor & editor)
	: m_Editor( editor )
	, m_Query()
{
}

void WorldWindow::Update(float deltaTime)
{
}

void WorldWindow::Draw()
{
	ImGui::Begin(GetName().c_str(), &m_visible, m_windowflags);

	EditorWorld * focusedWorld = m_Editor.GetFocusedWorld();
	if (focusedWorld != nullptr)
	{
		DrawWorld(focusedWorld->m_world);
	}

	ImGui::End();
}

void WorldWindow::DrawWorld(alvere::World & world)
{
	ImGui::Checkbox("Hide empty archetypes", &m_HideEmptyArchetypes);
	ImGui::InputText("Archetype Search", m_Query, 50, ImGuiInputTextFlags_AutoSelectAll);

	int archetypeIndex = 0;
	const auto & archetypes = world.GetArchetypes();
	for (auto & archetype : archetypes)
	{
		ImGui::PushID(archetypeIndex++);
		{
			std::string types = "";

			const auto & providerTypes = archetype.first.GetTypes();
			for (auto & type : providerTypes)
			{
				types += type.name();
				types += '\n';
			}

			bool visible = true;

			visible &= m_HideEmptyArchetypes == false || archetype.second->GetEntityCount() > 0;
			visible &= m_Query[0] == '\0' || types.find(m_Query) != std::string::npos;

			if (visible)
			{
				if (ImGui::CollapsingHeader(types.c_str()))
				{
					DrawArchetype(*archetype.second);
				}
			}
		}
		ImGui::PopID();
	}
}

void WorldWindow::DrawArchetype(alvere::Archetype & archetype)
{
	int index = 0;
	for (const auto & entity : archetype.GetEntities())
	{
		ImGui::PushID(index);
		{
			std::string name = std::to_string(index++);

			C_Name * nameContainer = archetype.TryGetComponent<C_Name>(entity);
			if (nameContainer != nullptr)
			{
				name = nameContainer->m_Name;
			}

			if (ImGui::TreeNode(name.c_str()))
			{
				DrawEntity(entity);
				ImGui::TreePop();
			}
		}
		ImGui::PopID();
	}
}

void WorldWindow::DrawEntity(const alvere::EntityHandle & entity)
{
	if (entity.isValid() == false)
	{
		ImGui::Text("Invalid entity handle");
	}

	alvere::Archetype & archetype = *entity->m_Archetype;

	const auto & providers = archetype.GetProviders();
	for (const auto & provider : providers)
	{
		const alvere::Component & component = provider.second->GetComponent(entity->m_MappingHandle.m_Index);

		std::string str = component.to_string();
		if (str == "")
		{
			float indent = ImGui::GetTreeNodeToLabelSpacing();
			ImGui::Indent(indent);
			{
				ImGui::Text(provider.first.name());
			}
			ImGui::Unindent(indent);
		}
		else if (ImGui::TreeNode(provider.first.name()))
		{
			float indent = ImGui::GetTreeNodeToLabelSpacing();
			ImGui::Indent(indent);
			{
				ImGui::Text(str.c_str());
				ImGui::TreePop();
			}
			ImGui::Unindent(indent);
		}
	}
}