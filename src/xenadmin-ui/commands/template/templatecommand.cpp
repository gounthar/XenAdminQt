/*
 * Copyright (c) 2026, Petr Bena <petr@bena.rocks>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "templatecommand.h"
#include "xenlib/xen/vm.h"
#include "xenlib/xen/network/connection.h"

TemplateCommand::TemplateCommand(MainWindow* mainWindow, QObject* parent) : Command(mainWindow, parent)
{
}

QString TemplateCommand::getSelectedTemplateRef() const
{
    if (this->getSelectedObjectType() != XenObjectType::VM)
        return QString();

    QSharedPointer<VM> templateVm = this->getTemplate();
    return templateVm ? templateVm->OpaqueRef() : QString();
}

QSharedPointer<VM> TemplateCommand::getTemplate() const
{
    QSharedPointer<XenObject> selectedObject = this->GetObject();
    if (!selectedObject || selectedObject->GetObjectType() != XenObjectType::VM)
        return QSharedPointer<VM>();

    return qSharedPointerDynamicCast<VM>(selectedObject);
}

QString TemplateCommand::getSelectedTemplateName() const
{
    QSharedPointer<VM> templateVm = this->getTemplate();
    return templateVm ? templateVm->GetName() : QString();
}

bool TemplateCommand::isTemplateSelected() const
{
    return !this->getSelectedTemplateRef().isEmpty();
}

bool TemplateCommand::canRunTemplate(const QSharedPointer<VM>& templateVm) const
{
    if (!templateVm)
        return false;

    XenConnection* connection = templateVm->GetConnection();
    if (!connection || !connection->IsConnected())
        return false;

    if (!templateVm->IsTemplate())
        return false;

    if (templateVm->IsSnapshot())
        return false;

    if (!templateVm->CurrentOperations().isEmpty())
        return false;

    return true;
}
