# Exclusion-blocker

## Overview

**Exclusion-blocker** is a simple Windows kernel-mode driver that prevents modifications to registry keys related to Windows Defender exclusions. This effectively blocks attempts to exclude specific files or directories from Defender scans — a technique frequently used by malware authors to bypass detection.

## How It Works

Exclusion-blocker hooks into the Windows registry filtering mechanism and monitors attempts to modify Defender's exclusion-related registry keys. When such an attempt is detected, it is blocked before the change is applied.

## Registry Keys Monitored

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Paths

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Extensions

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Processes

## Disclaimer

Exclusion-blocker is a proof-of-concept (PoC) project developed primarily for educational purposes. It is provided as-is, without any warranty of any kind. Use at your own risk, especially in production environments, as it has not been thoroughly tested for stability in real-world deployments.
