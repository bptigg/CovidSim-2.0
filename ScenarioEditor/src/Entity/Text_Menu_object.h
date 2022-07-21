#pragma once

#include "scriptable_object.h"
#include "Text.h"

class Text_Menu_object : public scriptable_object
{
private:
	Text m_text;
public:
	Text_Menu_object(Text text, glm::vec2 pos, Layer* layer);

	virtual void render() override;
	virtual void update() override;
};

