#!/bin/sh
# Build faeb minimalist foundation
set -e

echo "🔨 Building faeb minimalist foundation..."
echo "💙 Applying RISC-V paradigm principles"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check prerequisites
check_prerequisite() {
    if ! command -v $1 >/dev/null 2>&1; then
        echo "${RED}❌ $1 not found. Please install: $2${NC}"
        exit 1
    fi
    echo "${GREEN}✅ $1 found${NC}"
}

echo "${BLUE}🔍 Checking prerequisites...${NC}"
check_prerequisite gcc "https://gcc.gnu.org/install/"
check_prerequisite make "https://www.gnu.org/software/make/"
check_prerequisite python3 "https://www.python.org/downloads/"
check_prerequisite git "https://git-scm.com/downloads"

# Create build directory
mkdir -p build
cd build

# Build core runtime
echo "${BLUE}🔨 Building core runtime...${NC}"
mkdir -p core
cd core
cmake ../../core
make -j$(nproc)
cd ..

# Build extension interface
echo "${BLUE}🔨 Building extension interface...${NC}"
mkdir -p extend
cd extend
cmake ../../extend
make -j$(nproc)
cd ..

# Build verification framework
echo "${BLUE}🔨 Building verification framework...${NC}"
mkdir -p verify
cd verify
cmake ../../verify
make -j$(nproc)
cd ..

# Create distribution package
echo "${BLUE}📦 Creating distribution package...${NC}"
mkdir -p dist/faeb
cp core/faeb-runtime dist/faeb/
cp extend/faeb-extend dist/faeb/
cp verify/faeb-verify dist/faeb/
cp -r ../../examples dist/faeb/
cp -r ../../docs dist/faeb/

# Create tarball
tar -czf faeb-dist.tar.gz dist/

echo "${GREEN}✅ Build complete!${NC}"
echo "${GREEN}📦 Distribution: build/faeb-dist.tar.gz${NC}"
echo "${GREEN}💙 faeb minimalist foundation ready${NC}"
