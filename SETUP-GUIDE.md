# 🚀 FAEB Setup Guide
## Getting Started with Minimalist Foundation

> **Repository**: https://github.com/kae3g/faeb  
> **Identity**: b122m faeb internet identity  
> **Purpose**: Complete setup instructions for FAEB development

## 📋 Prerequisites

### Required Software
- **Git**: Version control system
- **GCC**: C compiler (version 4.9+)
- **Make**: Build system
- **CMake**: Build configuration (version 3.12+)
- **GPG**: Code signing and verification
- **AWS CLI**: For deployment (optional)
- **Python 3**: For build scripts (version 3.6+)

### Operating System Support
- **Linux**: Ubuntu 18.04+, CentOS 7+, Alpine Linux 3.10+
- **macOS**: 10.15+ (Catalina or later)
- **Windows**: Windows 10+ with WSL2 or MinGW

### Hardware Requirements
- **RAM**: Minimum 2GB, Recommended 4GB+
- **Storage**: 1GB free space
- **CPU**: Any x86_64 processor

## 🔑 Identity Setup (Required First)

### Step 1: Generate GPG Key
```bash
# Generate new GPG key
gpg --full-gen-key

# Choose options:
# 1. (9) ECC and ECC
# 2. (1) Curve 25519
# 3. 0 = key does not expire
# 4. Identity: "faeb (b122m faeb internet identity) <your-email@gmail.com>"
```

### Step 2: Configure Git Signing
```bash
# Set git configuration
git config --global user.name "faeb"
git config --global user.email "your-email@gmail.com"
git config --global user.signingkey YOUR_KEY_ID
git config --global commit.gpgsign true

# Get your key ID
gpg --list-secret-keys --keyid-format LONG
```

### Step 3: Export SSH Key
```bash
# Extract SSH key from GPG key
gpg --export-ssh-key your-email@gmail.com > ~/.ssh/id_ed25519.pub

# Add to SSH agent
ssh-add ~/.ssh/id_ed25519
```

## 📥 Installation

### Step 1: Clone Repository
```bash
# Clone FAEB repository
git clone https://github.com/kae3g/faeb.git
cd faeb

# Verify repository integrity
git verify-commit HEAD
```

### Step 2: Initialize Identity
```bash
# Run identity initialization script
./bin/init-identity

# This will:
# - Verify GPG key exists
# - Configure git for this repository
# - Export public keys to identity/ directory
# - Create identity manifest
```

### Step 3: Verify Installation
```bash
# Check prerequisites
./build.sh --check-deps

# Build project
./build.sh

# Run tests
./test.sh
```

## 🛠️ Development Environment Setup

### Linux Setup
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake git gnupg python3

# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake git gnupg python3

# Alpine Linux
sudo apk add --no-cache build-base cmake git gnupg python3
```

### macOS Setup
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake git gnupg python3
```

### Windows Setup
```bash
# Install WSL2 with Ubuntu
wsl --install

# In WSL2 Ubuntu:
sudo apt update
sudo apt install build-essential cmake git gnupg python3

# Or use MinGW:
# Download and install MinGW-w64
# Add to PATH: C:\mingw64\bin
```

## 🔨 Building FAEB

### Basic Build
```bash
# Build all components
./build.sh

# Build specific component
./build.sh --component core
./build.sh --component extend
./build.sh --component verify
```

### Advanced Build Options
```bash
# Debug build
./build.sh --debug

# Release build with optimizations
./build.sh --release

# Clean build
./build.sh --clean

# Parallel build
./build.sh --parallel
```

### Build Output
```
faeb/
├── build/
│   ├── core/
│   │   └── faeb-runtime.a
│   ├── extend/
│   │   └── faeb-extend.a
│   └── verify/
│       └── faeb-verify.a
└── dist/
    └── faeb-dist.tar.gz
```

## 🧪 Testing

### Run All Tests
```bash
# Execute complete test suite
./test.sh

# Run specific test categories
./test.sh --unit
./test.sh --integration
./test.sh --performance
```

### Test Coverage
```bash
# Generate coverage report
./test.sh --coverage

# View coverage report
open coverage/index.html
```

### Formal Verification
```bash
# Verify core components
./verify.sh --runtime

# Verify extension interface
./verify.sh --interface

# Generate proof certificates
./verify.sh --certificates
```

## 🌐 AWS Deployment Setup

### Prerequisites
- AWS account with appropriate permissions
- AWS CLI installed and configured

### Step 1: Configure AWS CLI
```bash
# Install AWS CLI
curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
unzip awscliv2.zip
sudo ./aws/install

# Configure credentials
aws configure
# Enter your AWS Access Key ID
# Enter your AWS Secret Access Key
# Enter default region (us-east-1 recommended)
# Enter default output format (json)
```

### Step 2: Deploy Website
```bash
# Deploy to AWS
./aws/deploy-website.sh

# This will:
# - Create S3 bucket
# - Upload website content
# - Create CloudFront distribution
# - Configure SSL certificate
```

### Step 3: Custom Domain Setup
```bash
# Follow Squarespace integration guide
cat aws/squarespace-domain-setup.md

# Update CloudFront for custom domain
aws cloudfront update-distribution --id YOUR_DISTRIBUTION_ID --distribution-config file://custom-domain-config.json
```

## 🔧 Configuration

### Build Configuration
```bash
# Edit build settings
vim core/CMakeLists.txt

# Key settings:
# - CMAKE_C_STANDARD: C standard version
# - CMAKE_CXX_STANDARD: C++ standard version
# - CMAKE_C_FLAGS: Compiler flags
# - CMAKE_CXX_FLAGS: C++ compiler flags
```

### Runtime Configuration
```bash
# Edit runtime settings
vim core/include/faeb/runtime.h

# Key settings:
# - Memory management options
# - Process scheduling parameters
# - I/O buffer sizes
# - Verification settings
```

### Extension Configuration
```bash
# Configure extensions
vim extend/extension.conf

# Settings:
# - Extension paths
# - Loading policies
# - Security restrictions
```

## 🐛 Troubleshooting

### Common Issues

#### Build Failures
```bash
# Check dependencies
./build.sh --check-deps

# Clean and rebuild
./build.sh --clean
./build.sh

# Verbose output
./build.sh --verbose
```

#### GPG Signing Issues
```bash
# Check GPG key
gpg --list-keys

# Test signing
echo "test" | gpg --clearsign

# Fix git configuration
git config --global user.signingkey YOUR_KEY_ID
```

#### AWS Deployment Issues
```bash
# Check AWS credentials
aws sts get-caller-identity

# Check S3 bucket
aws s3 ls

# Check CloudFront
aws cloudfront list-distributions
```

### Getting Help
```bash
# Check logs
tail -f build.log

# Debug mode
./build.sh --debug

# Verbose output
./build.sh --verbose
```

## 📚 Next Steps

### After Setup
1. **Read Documentation**: Start with `docs/principles.md`
2. **Explore Examples**: Check `examples/` directory
3. **Join Community**: Visit GitHub discussions
4. **Contribute**: Follow contribution guidelines

### Development Workflow
1. **Make Changes**: Edit source files
2. **Test Changes**: Run test suite
3. **Verify Changes**: Run verification
4. **Commit Changes**: Sign commits with GPG
5. **Push Changes**: Update repository

### Learning Resources
- **README**: https://github.com/kae3g/faeb/blob/main/README.md
- **Design Principles**: https://github.com/kae3g/faeb/blob/main/docs/principles.md
- **API Reference**: https://github.com/kae3g/faeb/blob/main/docs/api-reference.md
- **Tutorials**: https://github.com/kae3g/faeb/blob/main/docs/tutorials/

## 🔐 Security Considerations

### Code Signing
- All commits must be GPG signed
- Verify commits: `git verify-commit HEAD`
- Check repository integrity regularly

### Build Security
- Use reproducible builds
- Verify checksums
- Check for security vulnerabilities

### Deployment Security
- Use HTTPS only
- Configure security headers
- Monitor for security issues

## 📞 Support

### Getting Help
- **GitHub Issues**: https://github.com/kae3g/faeb/issues
- **Discussions**: https://github.com/kae3g/faeb/discussions
- **Documentation**: https://github.com/kae3g/faeb/docs

### Community Guidelines
- **Be Respectful**: Follow gentle revolution principles
- **Be Helpful**: Share knowledge and experience
- **Be Patient**: Learning takes time
- **Be Inclusive**: Welcome everyone

---
*This setup guide ensures you can get started with FAEB quickly and safely.* 💚🖤💛💙

**Generated**: 2025-09-13--2200--PACIFIC-WEST-STANDARD  
**Repository**: https://github.com/kae3g/faeb  
**Identity**: b122m faeb internet identity
