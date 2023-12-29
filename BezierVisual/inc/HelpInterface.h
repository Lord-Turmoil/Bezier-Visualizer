// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _BEZIERVISUAL_HELP_INTERFACE_H_
#define _BEZIERVISUAL_HELP_INTERFACE_H_

#include "Macros.h"


class HelpInterface : public AbstractInterface
{
public:
    void BindEvents() override;

private:
    static void _OpenGitHub();
};

#endif
