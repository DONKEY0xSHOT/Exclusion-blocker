# Exclusion-blocker

## Overview

**Exclusion-blocker** is a simple Windows kernel-mode driver that prevents modifications of registry keys related to Windows Defender exclusions. This effectively blocks attempts to exclude specific files or directories from Defender scans — a technique frequently used by malware authors to bypass detection.

## Registry Keys Monitored

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Paths

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Extensions

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Processes

## Disclaimer

Exclusion-blocker is a proof-of-concept (PoC) project developed primarily for educational purposes. It is provided as-is, without any warranty of any kind. Use at your own risk, especially in production environments, as it has not been thoroughly tested for stability in real-world deployments.
