# 🌐 Squarespace Domain Integration Guide
## Connecting Your Purchased TLD Domain to faeb Website

> **Generated**: 2025-09-13--2130--PACIFIC-WEST-STANDARD  
> **Purpose**: Guide for integrating purchased Squarespace domain with AWS-hosted faeb website

## 🎯 Overview

This guide walks you through connecting a domain purchased through Squarespace to your AWS-hosted faeb website. The process involves DNS configuration, CloudFront setup, and SSL certificate management.

## 📋 Prerequisites

### Required Information
- **Squarespace Domain**: Your purchased domain (e.g., `yourdomain.com`)
- **AWS Account**: Active AWS account with appropriate permissions
- **CloudFront Distribution ID**: From the AWS deployment (found in `aws-config.txt`)

### Required Tools
- AWS CLI configured with appropriate credentials
- Access to your Squarespace domain management panel
- SSL certificate for your domain (can be obtained through AWS Certificate Manager)

## 🚀 Step-by-Step Integration

### Step 1: Configure CloudFront for Custom Domain

#### 1.1 Add Custom Domain to CloudFront
```bash
# Get your CloudFront distribution ID from aws-config.txt
DISTRIBUTION_ID="YOUR_DISTRIBUTION_ID"
DOMAIN_NAME="yourdomain.com"

# Create CloudFront distribution configuration
aws cloudfront get-distribution-config --id $DISTRIBUTION_ID > current-config.json

# Edit the configuration to add your custom domain
# Add your domain to the "Aliases" section
```

#### 1.2 Update CloudFront Configuration
```bash
# Update the distribution with your custom domain
aws cloudfront update-distribution \
    --id $DISTRIBUTION_ID \
    --distribution-config file://updated-config.json
```

### Step 2: SSL Certificate Setup

#### 2.1 Request SSL Certificate
```bash
# Request certificate through AWS Certificate Manager
aws acm request-certificate \
    --domain-name $DOMAIN_NAME \
    --validation-method DNS \
    --region us-east-1
```

#### 2.2 Validate Certificate
```bash
# Get certificate ARN
CERT_ARN=$(aws acm list-certificates --region us-east-1 --query 'CertificateSummaryList[0].CertificateArn' --output text)

# Add DNS validation record to your domain
# (Instructions will be provided by AWS)
```

### Step 3: Squarespace DNS Configuration

#### 3.1 Access Squarespace Domain Settings
1. Log in to your Squarespace account
2. Navigate to **Settings** → **Domains**
3. Click on your domain name
4. Go to **DNS Settings**

#### 3.2 Add CNAME Record
Add a CNAME record with the following details:
- **Type**: CNAME
- **Host**: `www` (or leave blank for root domain)
- **Points to**: `YOUR_CLOUDFRONT_DISTRIBUTION_ID.cloudfront.net`
- **TTL**: 3600 (or default)

#### 3.3 Add A Record (Alternative)
If you prefer an A record instead of CNAME:
- **Type**: A
- **Host**: `@` (for root domain)
- **Points to**: CloudFront IP address (obtained from AWS)

### Step 4: Update CloudFront Distribution

#### 4.1 Add SSL Certificate to CloudFront
```bash
# Update CloudFront distribution with SSL certificate
aws cloudfront update-distribution \
    --id $DISTRIBUTION_ID \
    --distribution-config file://final-config.json
```

#### 4.2 Configure HTTPS Redirect
Ensure your CloudFront distribution redirects HTTP to HTTPS:
```json
{
    "ViewerProtocolPolicy": "redirect-to-https"
}
```

## 🔧 Advanced Configuration

### Custom Error Pages
```bash
# Configure custom error pages for better user experience
aws cloudfront update-distribution \
    --id $DISTRIBUTION_ID \
    --distribution-config file://error-pages-config.json
```

### Cache Optimization
```bash
# Optimize caching for static assets
aws cloudfront update-distribution \
    --id $DISTRIBUTION_ID \
    --distribution-config file://cache-optimization-config.json
```

### Security Headers
```bash
# Add security headers for enhanced protection
aws cloudfront update-distribution \
    --id $DISTRIBUTION_ID \
    --distribution-config file://security-headers-config.json
```

## 📊 Monitoring and Analytics

### CloudWatch Monitoring
```bash
# Set up CloudWatch alarms for your website
aws cloudwatch put-metric-alarm \
    --alarm-name "faeb-website-4xx-errors" \
    --alarm-description "Monitor 4xx errors on faeb website" \
    --metric-name 4xxErrorRate \
    --namespace AWS/CloudFront \
    --statistic Average \
    --period 300 \
    --threshold 5.0 \
    --comparison-operator GreaterThanThreshold
```

### Performance Monitoring
```bash
# Monitor website performance
aws cloudwatch put-metric-alarm \
    --alarm-name "faeb-website-response-time" \
    --alarm-description "Monitor response time on faeb website" \
    --metric-name ResponseTime \
    --namespace AWS/CloudFront \
    --statistic Average \
    --period 300 \
    --threshold 2.0 \
    --comparison-operator GreaterThanThreshold
```

## 🛠️ Troubleshooting

### Common Issues

#### 1. DNS Propagation
**Problem**: Domain not resolving to CloudFront
**Solution**: 
- Wait for DNS propagation (up to 48 hours)
- Check DNS records with `dig` or `nslookup`
- Verify CNAME record points to correct CloudFront domain

#### 2. SSL Certificate Issues
**Problem**: HTTPS not working
**Solution**:
- Verify certificate is issued and validated
- Check certificate is associated with CloudFront distribution
- Ensure certificate is in `us-east-1` region

#### 3. CloudFront Cache Issues
**Problem**: Changes not appearing on website
**Solution**:
```bash
# Invalidate CloudFront cache
aws cloudfront create-invalidation \
    --distribution-id $DISTRIBUTION_ID \
    --paths "/*"
```

### Diagnostic Commands

#### Check DNS Resolution
```bash
# Check DNS resolution
dig yourdomain.com
nslookup yourdomain.com
```

#### Test HTTPS Connection
```bash
# Test HTTPS connection
curl -I https://yourdomain.com
openssl s_client -connect yourdomain.com:443
```

#### Check CloudFront Status
```bash
# Check CloudFront distribution status
aws cloudfront get-distribution --id $DISTRIBUTION_ID
```

## 📋 Maintenance Tasks

### Regular Updates
- **SSL Certificate Renewal**: AWS Certificate Manager handles automatic renewal
- **Security Updates**: Regular security patches and updates
- **Performance Monitoring**: Weekly performance reviews
- **Backup Verification**: Monthly backup and recovery testing

### Monitoring Checklist
- [ ] Website accessibility from multiple locations
- [ ] SSL certificate validity
- [ ] CloudFront distribution health
- [ ] DNS resolution accuracy
- [ ] Performance metrics within acceptable ranges

## 🔐 Security Best Practices

### HTTPS Enforcement
- Ensure all traffic is redirected to HTTPS
- Use HSTS headers for enhanced security
- Implement CSP (Content Security Policy) headers

### Access Control
- Use AWS IAM for fine-grained access control
- Enable CloudTrail for audit logging
- Implement least privilege access principles

### Monitoring and Alerting
- Set up CloudWatch alarms for critical metrics
- Monitor for unusual traffic patterns
- Implement automated incident response

## 📞 Support and Resources

### AWS Support
- **AWS Documentation**: https://docs.aws.amazon.com/cloudfront/
- **AWS Support Center**: https://console.aws.amazon.com/support/
- **AWS Community Forums**: https://forums.aws.amazon.com/

### Squarespace Support
- **Squarespace Help Center**: https://support.squarespace.com/
- **Domain Management Guide**: https://support.squarespace.com/hc/en-us/articles/205815928

### faeb Community
- **GitHub Issues**: https://github.com/kae3g/faeb/issues
- **Community Discussions**: https://github.com/kae3g/faeb/discussions
- **Documentation**: https://github.com/kae3g/faeb/docs

## 🎯 Success Metrics

### Performance Targets
- **Page Load Time**: < 2 seconds
- **Uptime**: > 99.9%
- **SSL Grade**: A or higher
- **Mobile Performance**: > 90/100

### User Experience
- **Accessibility**: WCAG 2.1 AA compliance
- **Cross-browser Compatibility**: All major browsers
- **Mobile Responsiveness**: All device sizes

---
*This guide ensures your faeb website is properly integrated with your Squarespace domain while maintaining the highest standards of performance, security, and reliability.* 🌸💙

**Generated**: 2025-09-13--2130--PACIFIC-WEST-STANDARD  
**Repository**: https://github.com/kae3g/faeb  
**Identity**: b122m faeb internet identity
