# MyXAMPP — XAMPP-like Stack for Linux ARM64

## Quick Install
```bash
sudo bash install.sh
```

## Build from Source
```bash
bash build.sh
cd build && sudo ninja install
```

## GitHub Actions Workflow (ARM64)
```yaml
name: Build MyXAMPP ARM64
on: [push]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: docker/setup-qemu-action@v3
        with: { platforms: arm64 }
      - uses: docker/setup-buildx-action@v3
      - name: Build in ARM64 container
        run: |
          docker run --rm --platform linux/arm64 \
            -v $PWD:/work -w /work ubuntu:22.04 bash -c "
              apt-get update && apt-get install -y \
                build-essential cmake ninja-build \
                qt6-base-dev qt6-tools-dev libssl-dev
              mkdir build && cd build
              cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
              ninja -j$(nproc)
            "
      - uses: actions/upload-artifact@v4
        with:
          name: myxampp-arm64
          path: build/myxampp
```

## CLI Usage
```bash
/opt/myxampp/bin/myxamppctl start
/opt/myxampp/bin/myxamppctl status
/opt/myxampp/bin/myxamppctl stop
/opt/myxampp/bin/myxamppctl restart
```

## GUI
```bash
/opt/myxampp/bin/myxampp
```
