/*
 * Copyright (c) 2025, Petr Bena <petr@bena.rocks>
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

#include "instantvmfromtemplatecommand.h"
#include "../../mainwindow.h"
#include "xenlib/xen/vm.h"
#include <QMessageBox>

InstantVMFromTemplateCommand::InstantVMFromTemplateCommand(MainWindow* mainWindow, QObject* parent) : TemplateCommand(mainWindow, parent)
{
}

bool InstantVMFromTemplateCommand::CanRun() const
{
    return this->canRunTemplate(this->getTemplate());
}

void InstantVMFromTemplateCommand::Run()
{
    QSharedPointer<VM> templateVm = this->getTemplate();
    if (!this->canRunTemplate(templateVm))
    {
        QMessageBox::warning(MainWindow::instance(), "Cannot Create VM",
                             "The selected template cannot be used for instant VM creation.");
        return;
    }

    // TODO: Implement fast VM creation
    // CreateVMFastAction action = new CreateVMFastAction(connection, template);
    // action.Completed += (sender) => {
    //     if (action.Succeeded) {
    //         VMStartAction startAction = new VMStartAction(vm, ...);
    //         startAction.RunAsync();
    //     }
    // };
    // action.RunAsync();

    QMessageBox::information(MainWindow::instance(), "Not Implemented",
                             "Instant VM creation will be implemented using CreateVMFastAction + auto-start.");
}

QString InstantVMFromTemplateCommand::MenuText() const
{
    return "Instant VM from Template";
}

bool InstantVMFromTemplateCommand::canRunTemplate(const QSharedPointer<VM>& templateVm) const
{
    if (!TemplateCommand::canRunTemplate(templateVm))
        return false;

    // Must be an instant template
    if (!templateVm->IsInstantTemplate())
        return false;

    return true;
}
