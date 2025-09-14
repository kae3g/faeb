# 🌐 FAEB Website Deployment Guide
## AWS + Squarespace Domain Integration

> **Generated**: 2025-09-13--2145--PACIFIC-WEST-STANDARD  
> **Repository**: https://github.com/kae3g/faeb  
> **Identity**: b122m faeb internet identity

## 🎯 Overview

This guide walks you through deploying the FAEB minimalist foundation website to AWS and connecting it to a purchased Squarespace domain. The deployment includes:

- **AWS S3**: Static website hosting
- **CloudFront**: Global content delivery network
- **SSL Certificate**: Secure HTTPS connection
- **Custom Domain**: Squarespace domain integration
- **Monitoring**: Performance and availability tracking

## 📋 Prerequisites

### Required Accounts
- **AWS Account**: Active account with billing enabled
- **Squarespace Account**: With purchased domain
- **GitHub Account**: For repository access

### Required Tools
- AWS CLI configured with credentials
- Git for repository management
- GPG for signed commits

## 🚀 Step 1: AWS Setup

### 1.1 Configure AWS CLI
```bash
# Install AWS CLI (if not already installed)
curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
unzip awscliv2.zip
sudo ./aws/install

# Configure AWS credentials
aws configure
# Enter your AWS Access Key ID
# Enter your AWS Secret Access Key
# Enter your default region (us-east-1 recommended)
# Enter your default output format (json recommended)
```

### 1.2 Create IAM User (Recommended)
```bash
# Create IAM user for FAEB deployment
aws iam create-user --user-name faeb-deployer

# Attach policies for S3, CloudFront, and Certificate Manager
aws iam attach-user-policy --user-name faeb-deployer --policy-arn arn:aws:iam::aws:policy/AmazonS3FullAccess
aws iam attach-user-policy --user-name faeb-deployer --policy-arn arn:aws:iam::aws:policy/CloudFrontFullAccess
aws iam attach-user-policy --user-name faeb-deployer --policy-arn arn:aws:iam::aws:policy/CertificateManagerFullAccess

# Create access keys for the user
aws iam create-access-key --user-name faeb-deployer
```

## 🌐 Step 2: Deploy to AWS

### 2.1 Run Deployment Script
```bash
cd /Users/bhagavan851c05a/faeb
./aws/deploy-website.sh
```

The script will:
- Create S3 bucket for website hosting
- Upload website content
- Create CloudFront distribution
- Configure SSL certificate
- Set up monitoring

### 2.2 Manual Deployment (Alternative)
If the script fails, you can deploy manually:

```bash
# Create S3 bucket
BUCKET_NAME="faeb-website-$(date +%s)"
aws s3 mb s3://$BUCKET_NAME --region us-east-1

# Configure static website hosting
aws s3 website s3://$BUCKET_NAME --index-document index.html --error-document error.html

# Upload website content
aws s3 sync website/ s3://$BUCKET_NAME --region us-east-1

# Create CloudFront distribution
aws cloudfront create-distribution --distribution-config file://cloudfront-config.json
```

## 🔗 Step 3: Squarespace Domain Integration

### 3.1 Get CloudFront Distribution Domain
After deployment, note your CloudFront distribution domain:
```bash
# Get distribution domain from AWS config
cat aws-config.txt | grep WEBSITE_URL
```

### 3.2 Configure Squarespace DNS

#### Option A: CNAME Record (Recommended)
1. Log in to your Squarespace account
2. Go to **Settings** → **Domains**
3. Click on your domain
4. Go to **DNS Settings**
5. Add CNAME record:
   - **Host**: `www` (or leave blank for root domain)
   - **Points to**: `YOUR_CLOUDFRONT_DISTRIBUTION_ID.cloudfront.net`
   - **TTL**: 3600

#### Option B: A Record
1. Get CloudFront IP addresses:
   ```bash
   nslookup YOUR_CLOUDFRONT_DISTRIBUTION_ID.cloudfront.net
   ```
2. Add A records pointing to these IPs

### 3.3 Update CloudFront for Custom Domain
```bash
# Update CloudFront distribution to include your custom domain
aws cloudfront update-distribution \
    --id YOUR_DISTRIBUTION_ID \
    --distribution-config file://custom-domain-config.json
```

## 🔐 Step 4: SSL Certificate Setup

### 4.1 Request Certificate
```bash
# Request SSL certificate for your domain
aws acm request-certificate \
    --domain-name yourdomain.com \
    --subject-alternative-names www.yourdomain.com \
    --validation-method DNS \
    --region us-east-1
```

### 4.2 Validate Certificate
```bash
# Get certificate ARN
CERT_ARN=$(aws acm list-certificates --region us-east-1 --query 'CertificateSummaryList[0].CertificateArn' --output text)

# Add DNS validation records to Squarespace
# (Instructions will be provided by AWS Certificate Manager)
```

### 4.3 Associate Certificate with CloudFront
```bash
# Update CloudFront distribution with SSL certificate
aws cloudfront update-distribution \
    --id YOUR_DISTRIBUTION_ID \
    --distribution-config file://ssl-config.json
```

## 📊 Step 5: Monitoring Setup

### 5.1 CloudWatch Alarms
```bash
# Create alarm for 4xx errors
aws cloudwatch put-metric-alarm \
    --alarm-name "faeb-website-4xx-errors" \
    --alarm-description "Monitor 4xx errors on FAEB website" \
    --metric-name 4xxErrorRate \
    --namespace AWS/CloudFront \
    --statistic Average \
    --period 300 \
    --threshold 5.0 \
    --comparison-operator GreaterThanThreshold

# Create alarm for response time
aws cloudwatch put-metric-alarm \
    --alarm-name "faeb-website-response-time" \
    --alarm-description "Monitor response time on FAEB website" \
    --metric-name ResponseTime \
    --namespace AWS/CloudFront \
    --statistic Average \
    --period 300 \
    --threshold 2.0 \
    --comparison-operator GreaterThanThreshold
```

### 5.2 Performance Monitoring
```bash
# Set up Real User Monitoring (RUM)
aws cloudwatch put-metric-data \
    --namespace "FAEB/Website" \
    --metric-data MetricName=PageLoadTime,Value=1.5,Unit=Seconds
```

## 🧪 Step 6: Testing and Verification

### 6.1 Test Website Accessibility
```bash
# Test HTTP response
curl -I http://yourdomain.com

# Test HTTPS response
curl -I https://yourdomain.com

# Test CloudFront distribution
curl -I https://YOUR_DISTRIBUTION_ID.cloudfront.net
```

### 6.2 SSL Certificate Verification
```bash
# Check SSL certificate
openssl s_client -connect yourdomain.com:443 -servername yourdomain.com

# Verify certificate chain
curl -vI https://yourdomain.com 2>&1 | grep -E "(SSL|TLS|certificate)"
```

### 6.3 Performance Testing
```bash
# Test page load speed
curl -w "@curl-format.txt" -o /dev/null -s https://yourdomain.com

# Create curl-format.txt:
cat > curl-format.txt << EOF
     time_namelookup:  %{time_namelookup}\n
        time_connect:  %{time_connect}\n
     time_appconnect:  %{time_appconnect}\n
    time_pretransfer:  %{time_pretransfer}\n
       time_redirect:  %{time_redirect}\n
  time_starttransfer:  %{time_starttransfer}\n
                     ----------\n
          time_total:  %{time_total}\n
EOF
```

## 🛠️ Troubleshooting

### Common Issues

#### 1. DNS Propagation
**Problem**: Domain not resolving to CloudFront
**Solution**: 
- Wait for DNS propagation (up to 48 hours)
- Check DNS with `dig yourdomain.com`
- Verify CNAME record in Squarespace

#### 2. SSL Certificate Issues
**Problem**: HTTPS not working
**Solution**:
- Verify certificate is issued and validated
- Check certificate is in `us-east-1` region
- Ensure CloudFront distribution includes custom domain

#### 3. CloudFront Cache Issues
**Problem**: Changes not appearing
**Solution**:
```bash
# Invalidate CloudFront cache
aws cloudfront create-invalidation \
    --distribution-id YOUR_DISTRIBUTION_ID \
    --paths "/*"
```

### Diagnostic Commands
```bash
# Check DNS resolution
dig yourdomain.com
nslookup yourdomain.com

# Test connectivity
ping yourdomain.com
traceroute yourdomain.com

# Check HTTP headers
curl -I https://yourdomain.com
```

## 📋 Maintenance Tasks

### Regular Updates
- **SSL Certificate**: AWS Certificate Manager handles auto-renewal
- **Security Updates**: Regular security patches
- **Performance Monitoring**: Weekly performance reviews
- **Backup Verification**: Monthly backup testing

### Monitoring Checklist
- [ ] Website accessible from multiple locations
- [ ] SSL certificate valid and trusted
- [ ] CloudFront distribution healthy
- [ ] DNS resolution working correctly
- [ ] Performance metrics within acceptable ranges

## 🔐 Security Best Practices

### HTTPS Enforcement
- All traffic redirected to HTTPS
- HSTS headers implemented
- CSP (Content Security Policy) headers configured

### Access Control
- IAM users with least privilege access
- CloudTrail enabled for audit logging
- Regular security reviews

### Monitoring
- CloudWatch alarms for critical metrics
- Unusual traffic pattern detection
- Automated incident response

## 📞 Support and Resources

### AWS Support
- **AWS Documentation**: https://docs.aws.amazon.com/cloudfront/
- **AWS Support Center**: https://console.aws.amazon.com/support/
- **AWS Community**: https://forums.aws.amazon.com/

### Squarespace Support
- **Squarespace Help**: https://support.squarespace.com/
- **Domain Management**: https://support.squarespace.com/hc/en-us/articles/205815928

### FAEB Community
- **GitHub Issues**: https://github.com/kae3g/faeb/issues
- **Discussions**: https://github.com/kae3g/faeb/discussions
- **Documentation**: https://github.com/kae3g/faeb/docs

## 🎯 Success Metrics

### Performance Targets
- **Page Load Time**: < 2 seconds
- **Uptime**: > 99.9%
- **SSL Grade**: A or higher
- **Mobile Performance**: > 90/100

### User Experience
- **Accessibility**: WCAG 2.1 AA compliance
- **Cross-browser**: All major browsers supported
- **Mobile**: Responsive design on all devices

---
*This deployment guide ensures your FAEB website is properly integrated with AWS and your Squarespace domain while maintaining the highest standards of performance, security, and reliability.* 🌸💙

**Generated**: 2025-09-13--2145--PACIFIC-WEST-STANDARD  
**Repository**: https://github.com/kae3g/faeb  
**Identity**: b122m faeb internet identity
