#include "colour_palette.h"

colour_palette::colour_palette(glm::vec2 pos, Layer* layer, unsigned int render_layer, int id)
	:scriptable_object(pos, {256, 256}, layer, id), m_main(pos, {256, 256}, layer, true), m_secondary({pos.x + 150.0f, pos.y}, {32, 256}, layer, true)
{
	m_menu_object = true;
	m_type = entity_type::COLOUR_PALETTE;
	m_rendering_layer = render_layer;

	m_main.Bind_function(BIND_BUTTON_FN(colour_palette::get_rgb));
	m_secondary.Bind_function(BIND_BUTTON_FN(colour_palette::get_blue));

	red = 0.0f;
	blue = 0.0f;
	green = 0.0f;
}

colour_palette::~colour_palette()
{
}

void colour_palette::render()
{
	for (float r = 0; r < 256; r = r + 8)
	{
		for (float g = 0; g < 256; g = g + 8)
		{
			Renderer::draw_rectangle_color({ m_location.x - (m_size.x / 2.0f) + g, m_location.y - (m_size.y / 2.0f) + r }, { 16,16 }, { (r != 0) ? r / 256.0f : 0.0f, (g != 0) ? g / 256.0f : 0.0f, (blue != 0) ? blue / 256.0f : 0.0f, 1.0f }, m_rendering_layer, true);
		}
	}

	for (float b = 0; b < 256; b = b + 8)
	{
		Renderer::draw_rectangle_color({ m_location.x + 150.0f, m_location.y - (m_size.y / 2.0f) + b }, { 32, 16 }, { 0.0f, 0.0f, (b != 0) ? b / 256.0f : 0.0f, 1.0f}, m_rendering_layer, true);
	}
}

void colour_palette::update()
{
	render();
	//m_back.update();
}

void colour_palette::event_callback(Events::Event& e)
{
	m_main.event_callback(e);
	m_secondary.event_callback(e);
}

void colour_palette::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
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

	m_main.update_position(zoom, camera_pos, camera_mat);
	m_secondary.update_position(zoom, camera_pos, camera_mat);
}

void colour_palette::get_blue()
{
	m_secondary.p_is_mouse_over();
	blue = m_secondary.relative_position.y * 256;
}

void colour_palette::get_rgb()
{
	m_main.p_is_mouse_over();
	red = m_main.relative_position.x * 256;
	green = m_main.relative_position.y * 256;

	m_Layer->change_line_colour();
}

