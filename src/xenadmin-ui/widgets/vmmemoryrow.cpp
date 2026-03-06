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

#include "vmmemoryrow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

VMMemoryRow::VMMemoryRow(const QList<QSharedPointer<VM>>& vms, bool expanded, QWidget* parent)
    : QWidget(parent)
    , m_vms(vms)
    , m_expanded(expanded)
    , m_panelLabel(nullptr)
    , m_panelControls(nullptr)
    , m_vmMemoryControls(nullptr)
{
    this->SetupUi();
}

VMMemoryRow::~VMMemoryRow()
{
    this->UnregisterHandlers();
}

void VMMemoryRow::SetupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create label panel (top section with VM name)
    this->m_panelLabel = new QFrame(this);
    this->m_panelLabel->setFrameShape(QFrame::StyledPanel);
    this->m_panelLabel->setFrameShadow(QFrame::Raised);
    this->m_panelLabel->setLineWidth(1);
    this->m_panelLabel->setStyleSheet("background-color: silver;");
    
    QHBoxLayout* labelLayout = new QHBoxLayout(this->m_panelLabel);
    labelLayout->setContentsMargins(10, 5, 10, 5);
    labelLayout->setSpacing(10);
    
    // VM label (show first VM name or count if multiple)
    QLabel* vmLabel = new QLabel(this->m_panelLabel);
    if (!this->m_vms.isEmpty())
    {
        if (this->m_vms.size() == 1)
        {
            QSharedPointer<VM> vm = this->m_vms.first();
            if (vm && !vm->IsEvicted())
            {
                vmLabel->setText(vm->GetName());
            }
        }
        else
        {
            vmLabel->setText(tr("%1 VMs").arg(this->m_vms.size()));
        }
    }
    vmLabel->setStyleSheet("font-weight: bold;");
    labelLayout->addWidget(vmLabel);
    labelLayout->addStretch();
    
    mainLayout->addWidget(this->m_panelLabel);
    
    // Create controls panel (bottom section with memory info)
    this->m_panelControls = new QFrame(this);
    this->m_panelControls->setFrameShape(QFrame::StyledPanel);
    this->m_panelControls->setFrameShadow(QFrame::Raised);
    this->m_panelControls->setLineWidth(1);
    
    QHBoxLayout* controlsLayout = new QHBoxLayout(this->m_panelControls);
    controlsLayout->setContentsMargins(10, 10, 10, 10);
    controlsLayout->setSpacing(10);
    
    // Memory controls (right side)
    this->m_vmMemoryControls = new VMMemoryControls(this->m_panelControls);
    this->m_vmMemoryControls->SetVMs(this->m_vms);
    controlsLayout->addWidget(this->m_vmMemoryControls);
    
    mainLayout->addWidget(this->m_panelControls);
}

void VMMemoryRow::UnregisterHandlers()
{
    if (this->m_vmMemoryControls)
    {
        this->m_vmMemoryControls->UnregisterHandlers();
    }
}
