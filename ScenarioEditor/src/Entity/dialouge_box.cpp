#include "dialouge_box.h"

dialouge_box::dialouge_box(std::string text, const glm::vec2& pos, const glm::vec2& size, Layer* layer, unsigned int rendering_layer)
	:m_close("CLOSE", {pos.x, pos.y - (size.y / 2.0f) + 30.0f}, {64.0f, 32.0f}, layer, true, 35.0f), scriptable_object(pos, size, layer)
{
	m_rendering_layer = rendering_layer;

	m_menu_object = true;
	m_set_up = false;
	m_scale = size.y / 100;

	m_close.rendering_layer = rendering_layer;
	m_close.Bind_function(BIND_BUTTON_FN(dialouge_box::close));

	m_close.base_colour = { 0.15f, 0.15f, 0.15f, 1.0f };
	m_close.box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	m_close.selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };


	m_error_text = Text(text, { m_location.x, m_location.y - (10.0f * m_scale) }, 38.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, true);
	m_error = Text("ERROR", { m_location.x , m_location.y + (40.0f * m_scale) }, 50.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, true);

	m_type = entity_type::DIALOUGE_BOX;
}

dialouge_box::~dialouge_box()
{
}

void dialouge_box::update()
{
	m_close.update_position(m_zoom, m_camera_position, m_camera_matrix);
	
	render();
	m_close.update();
	m_error_text.render(0, 0, m_rendering_layer + 3, true);
	m_error.render(0, 0, m_rendering_layer + 3, true);

}

void dialouge_box::render()
{
	Renderer::draw_rectangle_color({ m_location.x, m_location.y + (40.0f * m_scale) }, { m_size.x, 20.0f * m_scale }, { 0.05f, 0.05f, 0.05f, 1.0f }, m_rendering_layer, m_menu_object);
	Renderer::draw_rectangle_color({ m_location.x, m_location.y - (10.0f * m_scale) }, { m_size.x, 80.0f * m_scale }, { 0.15f, 0.15f, 0.15f, 1.0f }, m_rendering_layer, m_menu_object);
	Renderer::draw_box({ m_location.x, m_location.y }, { m_size.x + 2.0f, m_size.y + 2.0f }, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, m_rendering_layer , m_menu_object);
}

void dialouge_box::event_callback(Events::Event& e)
{
	m_close.event_callback(e);
}

void dialouge_box::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
{
	if (!m_set_up)
	{
		m_zoom = zoom;
		m_camera_position = camera_pos;
		m_camera_matrix = camera_mat;
	}

	if (m_menu_object)
	{
		m_set_up = true;
	}
}

bool dialouge_box::on_mouse_move(Events::Mouse_Moved_Event& e)
{
	return false;
}

bool dialouge_box::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	return false;
}

void dialouge_box::close()
{
	m_delete_object = true;
}
