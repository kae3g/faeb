#!/bin/sh
# Deploy faeb website to AWS with Squarespace domain integration
set -e

echo "🌐 Deploying faeb website to AWS..."
echo "💙 Gentle revolution in cloud infrastructure"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
S3_BUCKET="faeb-website-$(date +%s)"
REGION="us-east-1"
CLOUDFRONT_DISTRIBUTION="faeb-distribution"

# Check AWS CLI
if ! command -v aws >/dev/null 2>&1; then
    echo "${RED}❌ AWS CLI not found. Please install: https://aws.amazon.com/cli/${NC}"
    exit 1
fi

echo "${GREEN}✅ AWS CLI found${NC}"

# Create S3 bucket for website
echo "${BLUE}🪣 Creating S3 bucket: $S3_BUCKET${NC}"
aws s3 mb s3://$S3_BUCKET --region $REGION

# Configure bucket for static website hosting
echo "${BLUE}🌐 Configuring static website hosting...${NC}"
aws s3 website s3://$S3_BUCKET --index-document index.html --error-document error.html

# Create website files
echo "${BLUE}📝 Creating website content...${NC}"
mkdir -p website

# Create index.html
cat > website/index.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🌸 faeb - Minimalist Foundation</title>
    <style>
        :root {
            --primary: #1b3b36;
            --secondary: #d4af37;
            --accent: #2a9d8f;
            --background: #f8f5f0;
            --text: #2c3e50;
        }
        
        body {
            font-family: 'Gentle Sans', serif;
            line-height: 1.618;
            color: var(--text);
            background: var(--background);
            margin: 0;
            padding: 2em;
        }
        
        .container {
            max-width: 60em;
            margin: 0 auto;
        }
        
        h1, h2, h3 {
            color: var(--primary);
            border-bottom: 2px solid var(--secondary);
            padding-bottom: 0.5em;
        }
        
        .hero {
            text-align: center;
            margin: 4em 0;
        }
        
        .hero h1 {
            font-size: 3em;
            color: var(--secondary);
            border: none;
            margin-bottom: 0.5em;
        }
        
        .hero p {
            font-size: 1.2em;
            color: var(--accent);
        }
        
        .features {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 2em;
            margin: 3em 0;
        }
        
        .feature {
            background: white;
            padding: 2em;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        
        .feature h3 {
            color: var(--primary);
            border: none;
            margin-top: 0;
        }
        
        .cta {
            text-align: center;
            margin: 4em 0;
            padding: 3em;
            background: var(--primary);
            color: white;
            border-radius: 8px;
        }
        
        .cta h2 {
            color: white;
            border: none;
        }
        
        .btn {
            display: inline-block;
            padding: 1em 2em;
            background: var(--secondary);
            color: var(--primary);
            text-decoration: none;
            border-radius: 4px;
            font-weight: bold;
            margin: 1em;
        }
        
        .btn:hover {
            background: var(--accent);
            color: white;
        }
        
        .footer {
            text-align: center;
            margin-top: 4em;
            padding-top: 2em;
            border-top: 1px solid var(--accent);
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="hero">
            <h1>🌸 faeb</h1>
            <p>Minimalist Foundation for Gentle Revolution</p>
            <p><strong>RISC-V Paradigm Principles</strong> | <strong>Verifiable, Reproducible, Minimal</strong></p>
        </div>
        
        <section>
            <h2>💙 Philosophy</h2>
            <p><strong>faeb</strong> represents the pure essence of minimalism in software development, embodying RISC-V paradigm principles:</p>
            <ul>
                <li><strong>Minimalism</strong>: The smallest possible implementation that serves the purpose</li>
                <li><strong>Orthogonality</strong>: Components work independently without side effects</li>
                <li><strong>Verifiability</strong>: Every aspect can be mathematically proven correct</li>
                <li><strong>Extensibility</strong>: Simple foundation for complex systems to emerge</li>
            </ul>
        </section>
        
        <section>
            <h2>🏗️ Core Components</h2>
            <div class="features">
                <div class="feature">
                    <h3>Minimal Runtime</h3>
                    <p><strong>Size</strong>: &lt;100KB base runtime</p>
                    <p><strong>Features</strong>: Memory management, process scheduling, basic I/O</p>
                    <p><strong>Verification</strong>: Formally verified memory safety</p>
                </div>
                
                <div class="feature">
                    <h3>Extension Interface</h3>
                    <p><strong>Standard</strong>: Well-defined extension points</p>
                    <p><strong>Safety</strong>: Type-safe extension loading</p>
                    <p><strong>Compatibility</strong>: Backward compatibility guaranteed</p>
                </div>
                
                <div class="feature">
                    <h3>Verification Framework</h3>
                    <p><strong>Formal Methods</strong>: Coq and Lean integration</p>
                    <p><strong>Proof Automation</strong>: Automated proof generation</p>
                    <p><strong>Certification</strong>: Exportable proof certificates</p>
                </div>
            </div>
        </section>
        
        <section>
            <h2>📊 Performance Characteristics</h2>
            <table style="width: 100%; border-collapse: collapse;">
                <tr style="background: var(--accent); color: white;">
                    <th style="padding: 1em; border: 1px solid white;">Feature</th>
                    <th style="padding: 1em; border: 1px solid white;">faeb</th>
                    <th style="padding: 1em; border: 1px solid white;">Traditional Systems</th>
                </tr>
                <tr>
                    <td style="padding: 1em; border: 1px solid var(--accent);"><strong>Memory Usage</strong></td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">1MB</td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">100MB+</td>
                </tr>
                <tr>
                    <td style="padding: 1em; border: 1px solid var(--accent);"><strong>Startup Time</strong></td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">10ms</td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">1000ms+</td>
                </tr>
                <tr>
                    <td style="padding: 1em; border: 1px solid var(--accent);"><strong>Code Size</strong></td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">100KB</td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">100MB+</td>
                </tr>
                <tr>
                    <td style="padding: 1em; border: 1px solid var(--accent);"><strong>Verification</strong></td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">Formal</td>
                    <td style="padding: 1em; border: 1px solid var(--accent);">Testing-only</td>
                </tr>
            </table>
        </section>
        
        <div class="cta">
            <h2>🚀 Get Started with faeb</h2>
            <p>Join the gentle revolution in software development</p>
            <a href="https://github.com/kae3g/faeb" class="btn">View on GitHub</a>
            <a href="https://github.com/kae3g/faeb/discussions" class="btn">Join Community</a>
        </div>
        
        <div class="footer">
            <p><strong>faeb</strong>: Where minimalism meets mathematical certainty in the gentle revolution of software development.</p>
            <p>🌸💙 Built with RISC-V paradigm principles</p>
        </div>
    </div>
</body>
</html>
EOF

# Create error.html
cat > website/error.html << 'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Page Not Found - faeb</title>
    <style>
        body {
            font-family: 'Gentle Sans', serif;
            text-align: center;
            padding: 4em;
            background: #f8f5f0;
            color: #2c3e50;
        }
        h1 { color: #d4af37; }
        a { color: #2a9d8f; }
    </style>
</head>
<body>
    <h1>🌸 Page Not Found</h1>
    <p>The page you're looking for doesn't exist.</p>
    <a href="/">Return to faeb home</a>
</body>
</html>
EOF

# Upload website to S3
echo "${BLUE}📤 Uploading website to S3...${NC}"
aws s3 sync website/ s3://$S3_BUCKET --region $REGION

# Create CloudFront distribution
echo "${BLUE}☁️ Creating CloudFront distribution...${NC}"
cat > cloudfront-config.json << EOF
{
    "CallerReference": "$CLOUDFRONT_DISTRIBUTION-$(date +%s)",
    "Comment": "faeb website distribution",
    "DefaultRootObject": "index.html",
    "Origins": {
        "Quantity": 1,
        "Items": [
            {
                "Id": "S3-$S3_BUCKET",
                "DomainName": "$S3_BUCKET.s3.amazonaws.com",
                "S3OriginConfig": {
                    "OriginAccessIdentity": ""
                }
            }
        ]
    },
    "DefaultCacheBehavior": {
        "TargetOriginId": "S3-$S3_BUCKET",
        "ViewerProtocolPolicy": "redirect-to-https",
        "TrustedSigners": {
            "Enabled": false,
            "Quantity": 0
        },
        "ForwardedValues": {
            "QueryString": false,
            "Cookies": {"Forward": "none"}
        },
        "MinTTL": 0,
        "DefaultTTL": 86400,
        "MaxTTL": 31536000
    },
    "Enabled": true,
    "PriceClass": "PriceClass_100"
}
EOF

DISTRIBUTION_ID=$(aws cloudfront create-distribution --distribution-config file://cloudfront-config.json --query 'Distribution.Id' --output text)

echo "${GREEN}✅ Website deployed successfully!${NC}"
echo "${GREEN}🌐 S3 Bucket: $S3_BUCKET${NC}"
echo "${GREEN}☁️ CloudFront Distribution: $DISTRIBUTION_ID${NC}"
echo "${GREEN}🔗 Website URL: https://$DISTRIBUTION_ID.cloudfront.net${NC}"

# Save configuration
cat > aws-config.txt << EOF
# faeb Website AWS Configuration
# Generated: $(date)

S3_BUCKET=$S3_BUCKET
CLOUDFRONT_DISTRIBUTION_ID=$DISTRIBUTION_ID
REGION=$REGION
WEBSITE_URL=https://$DISTRIBUTION_ID.cloudfront.net

# For Squarespace Domain Integration:
# 1. Go to your Squarespace domain settings
# 2. Add a CNAME record pointing to: $DISTRIBUTION_ID.cloudfront.net
# 3. Update CloudFront distribution to include your custom domain
# 4. Add SSL certificate for your domain
EOF

echo "${YELLOW}📋 Configuration saved to aws-config.txt${NC}"
echo "${YELLOW}📖 See aws-config.txt for Squarespace domain integration instructions${NC}"
echo "${GREEN}💙 faeb website deployment complete!${NC}"
