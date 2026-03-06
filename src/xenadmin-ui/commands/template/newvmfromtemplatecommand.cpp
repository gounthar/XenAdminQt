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

#include "newvmfromtemplatecommand.h"
#include "../../mainwindow.h"
#include "../vm/newvmcommand.h"
#include "xenlib/xen/vm.h"
#include "xenlib/xen/pool.h"
#include "xenlib/xen/network/connection.h"
#include "xenlib/xencache.h"
#include <QMessageBox>

NewVMFromTemplateCommand::NewVMFromTemplateCommand(MainWindow* mainWindow, QObject* parent) : TemplateCommand(mainWindow, parent)
{
}

bool NewVMFromTemplateCommand::CanRun() const
{
    return this->canRunTemplate(this->getTemplate());
}

void NewVMFromTemplateCommand::Run()
{
    QString templateRef = this->getSelectedTemplateRef();
    if (templateRef.isEmpty())
        return;

    QSharedPointer<VM> templateVm = this->getTemplate();
    if (!this->canRunTemplate(templateVm))
    {
        QMessageBox::warning(MainWindow::instance(), "Cannot Create VM", "The selected template cannot be used to create a VM.");
        return;
    }

    // Delegate to NewVMCommand to keep one wizard launch path.
    NewVMCommand* newVmCmd = new NewVMCommand(templateRef, MainWindow::instance(), this);
    newVmCmd->Run();
}

QString NewVMFromTemplateCommand::MenuText() const
{
    return "New VM from Template";
}

QIcon NewVMFromTemplateCommand::GetIcon() const
{
    return QIcon(":/icons/vm_create_16.png");
}

bool NewVMFromTemplateCommand::poolHasEnabledHosts() const
{
    QSharedPointer<VM> templateVm = this->getTemplate();
    if (!templateVm)
        return false;

    XenConnection* connection = templateVm->GetConnection();
    XenCache* cache = connection ? connection->GetCache() : nullptr;
    if (!cache)
        return false;

    QSharedPointer<Pool> pool = cache->GetPool();
    if (!pool)
        pool = cache->GetPoolOfOne();

    return pool && pool->HasEnabledHosts();
}

bool NewVMFromTemplateCommand::canRunTemplate(const QSharedPointer<VM>& templateVm) const
{
    if (!TemplateCommand::canRunTemplate(templateVm))
        return false;

    return this->poolHasEnabledHosts();
}
