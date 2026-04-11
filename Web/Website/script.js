// SSOEngine Website JavaScript

// Interactive 3D Elements Management
const interactive3D = {
    init() {
        this.bindEvents();
        this.startAnimations();
    },
    
    bindEvents() {
        // Hero 3D cube interaction
        const hero3d = document.getElementById('hero3d');
        if (hero3d) {
            hero3d.addEventListener('click', this.handle3DCubeClick.bind(this));
            hero3d.addEventListener('mouseenter', this.handle3DCubeHover.bind(this));
            hero3d.addEventListener('mouseleave', this.handle3DCubeLeave.bind(this));
        }
        
        // Navigation scroll effects
        this.setupScrollEffects();
        
        // Smooth scroll for navigation links
        this.setupSmoothScroll();
    },
    
    handle3DCubeClick() {
        const cube = document.getElementById('hero3d');
        cube.style.transform = 'translateY(-50%) scale(1.2) rotateX(720deg) rotateY(720deg)';
        setTimeout(() => {
            cube.style.transform = 'translateY(-50%) scale(1) rotateX(0deg) rotateY(0deg)';
        }, 1000);
        
        // Show fun message
        this.showNotification('🎮 Welcome to SSOEngine!');
    },
    
    handle3DCubeHover() {
        const faces = document.querySelectorAll('.cube-face');
        faces.forEach((face, index) => {
            face.style.transform += ` scale(1.1)`;
            face.style.background = `linear-gradient(135deg, rgba(${37 + index * 20}, ${99 + index * 10}, 235, 0.9), rgba(${59 + index * 15}, 130, 246, 0.7))`;
        });
    },
    
    handle3DCubeLeave() {
        const faces = document.querySelectorAll('.cube-face');
        faces.forEach((face) => {
            face.style.transform = face.style.transform.replace(' scale(1.1)', '');
            face.style.background = 'linear-gradient(135deg, rgba(37, 99, 235, 0.8), rgba(59, 130, 246, 0.6))';
        });
    },
    
    setupScrollEffects() {
        let lastScroll = 0;
        const navbar = document.querySelector('.navbar');
        
        window.addEventListener('scroll', () => {
            const currentScroll = window.pageYOffset;
            
            // Hide/show navbar on scroll
            if (currentScroll > lastScroll && currentScroll > 100) {
                navbar.style.transform = 'translateY(-100%)';
            } else {
                navbar.style.transform = 'translateY(0)';
            }
            
            // Add shadow on scroll
            if (currentScroll > 10) {
                navbar.style.boxShadow = '0 4px 30px rgba(0, 0, 0, 0.15)';
            } else {
                navbar.style.boxShadow = '0 2px 20px rgba(0, 0, 0, 0.1)';
            }
            
            lastScroll = currentScroll;
        });
    },
    
    setupSmoothScroll() {
        document.querySelectorAll('a[href^="#"]').forEach(anchor => {
            anchor.addEventListener('click', function (e) {
                e.preventDefault();
                const target = document.querySelector(this.getAttribute('href'));
                if (target) {
                    target.scrollIntoView({
                        behavior: 'smooth',
                        block: 'start'
                    });
                }
            });
        });
    },
    
    startAnimations() {
        // Animate floating elements on scroll
        this.animateOnScroll();
        
        // Add parallax effect
        this.setupParallax();
    },
    
    animateOnScroll() {
        const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    entry.target.style.animation = 'fadeInUp 0.8s ease-out forwards';
                }
            });
        }, { threshold: 0.1 });
        
        document.querySelectorAll('.feature-card, .tool-card, .doc-card').forEach(el => {
            observer.observe(el);
        });
    },
    
    setupParallax() {
        window.addEventListener('scroll', () => {
            const scrolled = window.pageYOffset;
            const parallax = document.querySelector('.hero');
            const speed = 0.5;
            
            if (parallax) {
                parallax.style.transform = `translateY(${scrolled * speed}px)`;
            }
        });
    },
    
    showNotification(message) {
        const notification = document.createElement('div');
        notification.className = 'notification';
        notification.textContent = message;
        notification.style.cssText = `
            position: fixed;
            top: 100px;
            right: 20px;
            background: linear-gradient(135deg, var(--primary-color), var(--primary-dark));
            color: white;
            padding: 15px 25px;
            border-radius: 8px;
            box-shadow: 0 4px 20px rgba(37, 99, 235, 0.3);
            z-index: 10000;
            animation: slideInRight 0.5s ease-out;
            font-weight: 600;
        `;
        
        document.body.appendChild(notification);
        
        setTimeout(() => {
            notification.style.animation = 'slideOutRight 0.5s ease-out forwards';
            setTimeout(() => {
                document.body.removeChild(notification);
            }, 500);
        }, 3000);
    }
};

// Cookie Consent Management
const cookieConsent = {
    init() {
        this.checkConsent();
        this.bindEvents();
    },
    
    checkConsent() {
        const consent = localStorage.getItem('cookieConsent');
        if (!consent) {
            this.showBanner();
        }
    },
    
    showBanner() {
        setTimeout(() => {
            const banner = document.getElementById('cookieBanner');
            if (banner) {
                banner.classList.add('show');
            }
        }, 1000);
    },
    
    hideBanner() {
        const banner = document.getElementById('cookieBanner');
        if (banner) {
            banner.classList.remove('show');
            banner.classList.add('hide');
            setTimeout(() => {
                banner.style.display = 'none';
            }, 300);
        }
    },
    
    acceptAll() {
        localStorage.setItem('cookieConsent', 'all');
        this.hideBanner();
        this.enableAnalytics();
    },
    
    acceptNecessary() {
        localStorage.setItem('cookieConsent', 'necessary');
        this.hideBanner();
        this.disableAnalytics();
    },
    
    enableAnalytics() {
        // Enable analytics cookies here
        console.log('Analytics cookies enabled');
        // You can add Google Analytics or other tracking here
    },
    
    disableAnalytics() {
        // Disable analytics cookies here
        console.log('Only necessary cookies enabled');
        // Ensure no analytics tracking is loaded
    },
    
    bindEvents() {
        const acceptAllBtn = document.getElementById('acceptAll');
        const acceptNecessaryBtn = document.getElementById('acceptNecessary');
        
        if (acceptAllBtn) {
            acceptAllBtn.addEventListener('click', () => this.acceptAll());
        }
        
        if (acceptNecessaryBtn) {
            acceptNecessaryBtn.addEventListener('click', () => this.acceptNecessary());
        }
    }
};

// Initialize cookie consent
cookieConsent.init();

// Navigation Toggle
const navToggle = document.getElementById('navToggle');
const navMenu = document.querySelector('.nav-menu');

navToggle.addEventListener('click', () => {
    navMenu.classList.toggle('active');
    
    // Animate hamburger menu
    const spans = navToggle.querySelectorAll('span');
    spans.forEach((span, index) => {
        if (navMenu.classList.contains('active')) {
            if (index === 0) span.style.transform = 'rotate(45deg) translateY(8px)';
            if (index === 1) span.style.opacity = '0';
            if (index === 2) span.style.transform = 'rotate(-45deg) translateY(-8px)';
        } else {
            span.style.transform = '';
            span.style.opacity = '';
        }
    });
});

// Close mobile menu when clicking on a link
document.querySelectorAll('.nav-link').forEach(link => {
    link.addEventListener('click', () => {
        navMenu.classList.remove('active');
        const spans = navToggle.querySelectorAll('span');
        spans.forEach(span => {
            span.style.transform = '';
            span.style.opacity = '';
        });
    });
});

// Smooth scrolling for navigation links
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
        }
    });
});

// Navbar background on scroll
window.addEventListener('scroll', () => {
    const navbar = document.querySelector('.navbar');
    if (window.scrollY > 50) {
        navbar.style.background = 'rgba(10, 14, 39, 0.98)';
        navbar.style.boxShadow = '0 4px 20px rgba(0, 0, 0, 0.3)';
    } else {
        navbar.style.background = 'rgba(10, 14, 39, 0.95)';
        navbar.style.boxShadow = 'none';
    }
});

// Intersection Observer for fade-in animations
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -50px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.style.opacity = '1';
            entry.target.style.transform = 'translateY(0)';
        }
    });
}, observerOptions);

// Observe elements for animation
document.querySelectorAll('.feature-card, .tool-card, .doc-card').forEach(el => {
    el.style.opacity = '0';
    el.style.transform = 'translateY(30px)';
    el.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
    observer.observe(el);
});

// Typing effect for hero description
const typeText = (element, text, speed = 50) => {
    let i = 0;
    element.textContent = '';
    
    const type = () => {
        if (i < text.length) {
            element.textContent += text.charAt(i);
            i++;
            setTimeout(type, speed);
        }
    };
    
    type();
};

// Initialize typing effect when page loads
window.addEventListener('load', () => {
    const heroDescription = document.querySelector('.hero-description');
    if (heroDescription) {
        const originalText = heroDescription.textContent;
        typeText(heroDescription, originalText, 30);
    }
});

// Button hover effects
document.querySelectorAll('.btn').forEach(btn => {
    btn.addEventListener('mouseenter', function(e) {
        const rect = this.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        
        const ripple = document.createElement('span');
        ripple.style.position = 'absolute';
        ripple.style.width = '0';
        ripple.style.height = '0';
        ripple.style.borderRadius = '50%';
        ripple.style.background = 'rgba(255, 255, 255, 0.3)';
        ripple.style.transform = 'translate(-50%, -50%)';
        ripple.style.left = x + 'px';
        ripple.style.top = y + 'px';
        ripple.style.animation = 'ripple 0.6s ease-out';
        ripple.style.pointerEvents = 'none';
        
        this.style.position = 'relative';
        this.style.overflow = 'hidden';
        this.appendChild(ripple);
        
        setTimeout(() => ripple.remove(), 600);
    });
});

// Add ripple animation
const style = document.createElement('style');
style.textContent = `
    @keyframes ripple {
        to {
            width: 200px;
            height: 200px;
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);

// Stats counter animation
const animateCounter = (element, target, duration = 2000) => {
    const start = 0;
    const increment = target / (duration / 16);
    let current = start;
    
    const updateCounter = () => {
        current += increment;
        if (current < target) {
            element.textContent = Math.floor(current) + (element.textContent.includes('+') ? '+' : '');
            requestAnimationFrame(updateCounter);
        } else {
            element.textContent = target + (element.textContent.includes('+') ? '+' : '');
        }
    };
    
    updateCounter();
};

// Initialize counter animation when stats are visible
const statsObserver = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting && !entry.target.classList.contains('animated')) {
            const statNumber = entry.target.querySelector('.stat-number');
            const text = statNumber.textContent;
            const number = parseInt(text.replace(/\D/g, ''));
            
            if (!isNaN(number)) {
                animateCounter(statNumber, number);
                entry.target.classList.add('animated');
            }
        }
    });
}, { threshold: 0.5 });

document.querySelectorAll('.stat').forEach(stat => {
    statsObserver.observe(stat);
});

// Code syntax highlighting (simple version)
document.querySelectorAll('.code-content code').forEach(codeBlock => {
    const code = codeBlock.textContent;
    let highlightedCode = code;
    
    // Simple syntax highlighting
    highlightedCode = highlightedCode.replace(/\b(include|#include)\b/g, '<span style="color: #ff6b6b;">$1</span>');
    highlightedCode = highlightedCode.replace(/\b(int|float|void|bool|char|double)\b/g, '<span style="color: #4ecdc4;">$1</span>');
    highlightedCode = highlightedCode.replace(/\b(SSO::\w+)\b/g, '<span style="color: #00a8ff;">$1</span>');
    highlightedCode = highlightedCode.replace(/\/\/.*$/gm, '<span style="color: #6c7293; font-style: italic;">$&</span>');
    highlightedCode = highlightedCode.replace(/(".*?")/g, '<span style="color: #28a745;">$1</span>');
    
    codeBlock.innerHTML = highlightedCode;
});

// Copy code functionality
document.querySelectorAll('.code-preview').forEach(preview => {
    const copyBtn = document.createElement('button');
    copyBtn.className = 'copy-btn';
    copyBtn.innerHTML = '📋 Copy';
    copyBtn.style.cssText = `
        position: absolute;
        top: 10px;
        right: 60px;
        background: rgba(0, 168, 255, 0.2);
        border: 1px solid var(--primary-color);
        color: var(--primary-color);
        padding: 6px 12px;
        border-radius: 4px;
        font-size: 0.75rem;
        cursor: pointer;
        transition: all 0.3s ease;
    `;
    
    preview.style.position = 'relative';
    preview.appendChild(copyBtn);
    
    copyBtn.addEventListener('click', () => {
        const code = preview.querySelector('.code-content code').textContent;
        navigator.clipboard.writeText(code).then(() => {
            copyBtn.innerHTML = '✅ Copied!';
            copyBtn.style.background = 'rgba(40, 167, 69, 0.2)';
            copyBtn.style.borderColor = '#28a745';
            copyBtn.style.color = '#28a745';
            
            setTimeout(() => {
                copyBtn.innerHTML = '📋 Copy';
                copyBtn.style.background = 'rgba(0, 168, 255, 0.2)';
                copyBtn.style.borderColor = 'var(--primary-color)';
                copyBtn.style.color = 'var(--primary-color)';
            }, 2000);
        });
    });
    
    copyBtn.addEventListener('mouseenter', () => {
        copyBtn.style.background = 'rgba(0, 168, 255, 0.3)';
    });
    
    copyBtn.addEventListener('mouseleave', () => {
        if (!copyBtn.innerHTML.includes('✅')) {
            copyBtn.style.background = 'rgba(0, 168, 255, 0.2)';
        }
    });
});

// Parallax effect for hero section
window.addEventListener('scroll', () => {
    const scrolled = window.pageYOffset;
    const hero = document.querySelector('.hero');
    const heroContent = document.querySelector('.hero-content');
    const heroVisual = document.querySelector('.hero-visual');
    
    if (hero && heroContent && heroVisual) {
        heroContent.style.transform = `translateY(${scrolled * 0.3}px)`;
        heroVisual.style.transform = `translateY(${scrolled * 0.5}px)`;
    }
});

// Tool card hover effect with 3D transform
document.querySelectorAll('.tool-card').forEach(card => {
    card.addEventListener('mousemove', (e) => {
        const rect = card.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        
        const centerX = rect.width / 2;
        const centerY = rect.height / 2;
        
        const rotateX = (y - centerY) / 10;
        const rotateY = (centerX - x) / 10;
        
        card.style.transform = `perspective(1000px) rotateX(${rotateX}deg) rotateY(${rotateY}deg) translateZ(10px)`;
    });
    
    card.addEventListener('mouseleave', () => {
        card.style.transform = 'perspective(1000px) rotateX(0) rotateY(0) translateZ(0)';
    });
});

// Form validation (for future contact forms)
const validateEmail = (email) => {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
};

// Smooth reveal for sections
const revealSection = () => {
    const sections = document.querySelectorAll('section');
    
    sections.forEach(section => {
        const sectionTop = section.getBoundingClientRect().top;
        const sectionHeight = section.clientHeight;
        const scrollPosition = window.pageYOffset + window.innerHeight;
        
        if (scrollPosition > sectionTop + sectionHeight * 0.2) {
            section.classList.add('revealed');
        }
    });
};

window.addEventListener('scroll', revealSection);
window.addEventListener('load', revealSection);

// Performance optimization: Debounce scroll events
const debounce = (func, wait) => {
    let timeout;
    return function executedFunction(...args) {
        const later = () => {
            clearTimeout(timeout);
            func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
    };
};

// Apply debounce to scroll events
window.addEventListener('scroll', debounce(() => {
    // Scroll-related functions
}, 10));

// Dark mode toggle (if needed in future)
const toggleDarkMode = () => {
    document.body.classList.toggle('light-mode');
    localStorage.setItem('darkMode', document.body.classList.contains('light-mode') ? 'false' : 'true');
};

// Check for saved dark mode preference
if (localStorage.getItem('darkMode') === 'false') {
    document.body.classList.add('light-mode');
}

// Console welcome message
console.log('%c🎮 Welcome to SSOEngine Website!', 'color: #00a8ff; font-size: 20px; font-weight: bold;');
console.log('%cSimple 2D Game Engine Built with Raylib', 'color: #b8bcc8; font-size: 14px;');
console.log('%c🔗 GitHub: https://github.com/Rozaq125/SSOEngine', 'color: #4ecdc4; font-size: 12px;');

// Error handling for missing assets
document.addEventListener('error', (e) => {
    if (e.target.tagName === 'IMG') {
        e.target.style.display = 'none';
        console.warn('Image not found:', e.target.src);
    }
}, true);

// Initialize everything when DOM is loaded
document.addEventListener('DOMContentLoaded', function() {
    // Initialize interactive 3D elements
    interactive3D.init();
    
    // Initialize cookie consent
    cookieConsent.init();
    
    // Initialize navigation
    navigation.init();
    
    // Initialize stats counter
    statsCounter.init();
    
    // Initialize mobile menu
    mobileMenu.init();
    
    console.log('🎮 SSOEngine Website Initialized Successfully!');
    
    // Add loading animation removal
    setTimeout(() => {
        document.body.classList.add('loaded');
    }, 100);
});

// Export functions for potential use
window.SSOEngine = {
    toggleDarkMode,
    validateEmail,
    animateCounter,
    typeText
};
