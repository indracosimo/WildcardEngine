#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D iChannel0; // Input texture
uniform vec2 iResolution; // Viewport resolution (in pixels)
uniform float iTime; // Time in seconds

//Scanline params
float hardScan = -8.0; //hardness of scanlines, -8 is soft, -16 is medium etc...
float hardPix = -3.0; //hardness of pixels in scanlines, -2 is soft, -4 is hard
vec2 warp = vec2(1.0/32.0,1.0/24.0); //displays warp, 0.0 = none, 1.0/8.0 is extreme

//amount of shadow mask
float maskDark = 0.5; 
float maskLight = 1.5;

//------------------------------------------------------------------------

float ToLinear1(float c)
{
    return(c<=0.0405) ? c/12.92 : pow((c+0.055)/1.055,2.4);
}
vec3 ToLinear(vec3 c)
{
    return vec3(ToLinear1(c.r),ToLinear1(c.g),ToLinear1(c.b));
}


float ToSrgb1(float c)
{
    return(c<0.0031308?c*12.92:1.055*pow(c,0.41666)-0.055);
}

vec3 ToSrgb(vec3 c)
{
    return vec3(ToSrgb1(c.r),ToSrgb1(c.g),ToSrgb1(c.b));
}

vec3 Fetch(vec2 pos)
{
    if(pos.x < 0.0 || pos.y < 0.0 || pos.x > 1.0 || pos.y > 1.0) return vec3(0.0);
    return ToLinear(texture(iChannel0, pos).rgb);
}

vec2 Dist(vec2 pos)
{
    pos *= iResolution;
    return -((pos-floor(pos))-vec2(0.5));
}

float Gaus(float pos, float scale)
{
    return exp(scale*pos*pos*1.75);
}

//a=-2, b=-1, c=0, d=1, e=2, w=gaussian weight
vec3 Horz3(vec2 pos, float off)
{
    vec3 b = Fetch(pos + vec2(-1.5, off)/iResolution);
    vec3 c = Fetch(pos + vec2( 0.0, off)/iResolution);
    vec3 d = Fetch(pos + vec2( 1.5, off)/iResolution);
    float dst = Dist(pos).x;
    float wb = Gaus(dst-1.0, hardPix);
    float wc = Gaus(dst+0.0, hardPix);
    float wd = Gaus(dst+1.0, hardPix);
    return (b*wb + c*wc + d*wd)/(wb+wc+wd);
}

vec3 Horz5(vec2 pos, float off)
{
    vec3 a = Fetch(pos + vec2(-2.0, off)/iResolution);
    vec3 b = Fetch(pos + vec2(-1.0, off)/iResolution);
    vec3 c = Fetch(pos + vec2( 0.0, off)/iResolution);
    vec3 d = Fetch(pos + vec2( 1.0, off)/iResolution);
    vec3 e = Fetch(pos + vec2( 2.0, off)/iResolution);
    float dst = Dist(pos).x;
    float wa = Gaus(dst-2.0, hardPix);
    float wb = Gaus(dst-1.0, hardPix);
    float wc = Gaus(dst+0.0, hardPix);
    float wd = Gaus(dst+1.0, hardPix);
    float we = Gaus(dst+2.0, hardPix);
    return (a*wa + b*wb + c*wc + d*wd + e*we)/(wa+wb+wc+wd+we);
}

float Scanline(vec2 pos, float off)
{
    return Gaus(Dist(pos).y + off, hardScan);
}

vec3 Tri(vec2 pos)
{
    vec3 a = Horz3(pos, -1.0);
    vec3 b = Horz5(pos,  0.0); 
    vec3 c = Horz3(pos,  1.0);
    float wa = Scanline(pos, -1.0);
    float wb = Scanline(pos,  0.0);
    float wc = Scanline(pos,  1.0);
    return a*wa + b*wb + c*wc;
}

vec2 Warp(vec2 pos)
{
    pos = pos*2.0 - 1.0;
    pos *= vec2(1.0+(pos.y*pos.y)*warp.x, 1.0+(pos.x*pos.x)*warp.y);
    return pos*0.5 + 0.5;
}

vec3 Mask(vec2 pos)
{
    pos.x += pos.y*3.0;
    vec3 mask = vec3(maskDark);
    pos.x = fract(pos.x/6.0);
    if(pos.x < 0.333) mask.r = maskLight;
    else if(pos.x < 0.666) mask.g = maskLight;
    else mask.b = maskLight;
    return mask;
}

float Bar(float pos, float bar)
{ 
    pos -= bar; return pos*pos < 4.0 ? 0.0 : 1.0; 
}

vec3 RadialBlur(vec2 pos, vec3 col)
{
    float total = 0.0;
    vec3 ret = vec3(0.0);
    float radius = 2.0;
    int samples = 8;
    
    for(int i = 0; i < samples; i++)
    {
        float angle = 2.0 * 3.14159 * float(i) / float(samples);
        vec2 offset = vec2(cos(angle), sin(angle)) * radius / iResolution.xy;
        float weight = 1.0 / float(samples);
        ret += Fetch(pos + offset) * weight;
        total += weight;
    }
    
    return mix(col, ret / total, 0.5);
}

vec3 SoftenEdges(vec3 color, vec2 uv)
{
    vec2 center = uv * 2.0 - 1.0;
    float radius = length(center);
    float softness = 0.15;
    float edge_fade = smoothstep(1.0 - softness, 1.0, radius);
    return color * (1.0 - edge_fade);
}

vec3 BlendNeighbors(vec2 pos)
{
    vec2 pixelSize = 1.0 / iResolution.xy;
    float bleedRadius = 2.0;
    float bleedStrength = 1.5;

    //sample center and neighbors
    vec3 center = Fetch(pos);
    vec3 left = Fetch(pos - vec2(pixelSize.x * bleedRadius, 0.0));
    vec3 right = Fetch(pos + vec2(pixelSize.x * bleedRadius, 0.0));
    vec3 top = Fetch(pos + vec2(0.0, pixelSize.y * bleedRadius));
    vec3 bottom = Fetch(pos - vec2(0.0, pixelSize.y * bleedRadius));

    //diagonal samples for stronger bleed
    vec3 topLeft = Fetch(pos + vec2(-pixelSize.x * bleedRadius, pixelSize.y * bleedRadius));
    vec3 topRight = Fetch(pos + vec2(pixelSize.x * bleedRadius, pixelSize.y * bleedRadius));
    vec3 bottomLeft = Fetch(pos + vec2(-pixelSize.x * bleedRadius, -pixelSize.y * bleedRadius));
    vec3 bottomRight = Fetch(pos + vec2(pixelSize.x * bleedRadius, -pixelSize.y * bleedRadius));

    //average all samples
    vec2 frac = fract(pos * iResolution.xy);

    //blending weights
    float threshold = 0.05;
    vec3 horizDiff = abs(left - right) * bleedStrength;
    vec3 vertDiff = abs(top - bottom) * bleedStrength;
    vec3 diagDiff = (abs(topLeft - bottomRight) + abs(topRight - bottomLeft)) * bleedStrength;

    vec3 blendedColor = center;

    if(length(horizDiff) > threshold || length(vertDiff) > threshold || length(diagDiff) > threshold)
    {
        float horizontalWeight = smoothstep(0.0, 1.0, length(horizDiff));
        float verticalWeight = smoothstep(0.0, 1.0, length(vertDiff));
        float diagonalWeight = smoothstep(0.0, 1.0, length(diagDiff));

        vec3 horizontalBlend = mix(mix(left, center, frac.x), mix(center, right, frac.x), frac.x);
        vec3 verticalBlend = mix(mix(bottom, center, frac.y), mix(center, top, frac.y), frac.y);
        vec3 diagonalBlend = mix(mix(bottomLeft, topRight, frac.x), mix(bottomRight, topLeft, frac.x), frac.y);

        //combine blends based on weights
        blendedColor = mix(center, (horizontalBlend * horizontalWeight + verticalBlend * verticalWeight + diagonalBlend * diagonalWeight) 
        /
        (horizontalWeight + verticalWeight + diagonalWeight), max(max(horizontalWeight, verticalWeight), diagonalWeight));
    }

    vec3 glow = vec3(0.0);
    float glowRadius = 3.0;
    for(float i = -glowRadius; i <= glowRadius; i += 1.0)
    {
        for(float j = -glowRadius; j <= glowRadius; j += 1.0)
        {
            vec2 offset = vec2(i, j) * pixelSize;
            float weight = 1.0 - length(offset) / (glowRadius * length(pixelSize));
            if(weight > 0.0)
            {
                glow += Fetch(pos + offset) * weight * 0.1;
            }
        }
    }

    glow /= (glowRadius * glowRadius * 4.0);

    return mix(blendedColor, glow, 0.2);
}

void main()
{
    //base values
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec3 color;

    color = BlendNeighbors(uv) * Mask(gl_FragCoord.xy);
    color = RadialBlur(uv, color);

    FragColor = vec4(ToSrgb(color), 1.0);
}
//////////////////////////////////////////////////////////////////////////////
//#version 330 core
//out vec4 FragColor;
//in vec2 TexCoord;

//uniform sampler2D iChannel0;
//uniform vec2 iResolution;
//uniform float iTime;

//// cheap params
//const float SCAN_INTENSITY = 0.08;
//const float MASK_DARK = 0.6;
//const float MASK_LIGHT = 1.3;
//const float GLOW_STRENGTH = 0.12;


//vec3 blurSample(vec2 uv, vec2 texelSize)
//{
//    vec3 color = texture(iChannel0, uv).rgb * 0.36;
//    color += texture(iChannel0, uv + vec2(texelSize.x * 1.0, 0.0)).rgb * 0.12;
//    color += texture(iChannel0, uv + vec2(-texelSize.x * 1.0, 0.0)).rgb * 0.12;
//    color += texture(iChannel0, uv + vec2(0.0, texelSize.y * 1.0)).rgb * 0.12;
//    color += texture(iChannel0, uv + vec2(0.0, -texelSize.y * 1.0)).rgb * 0.12;
//    color += texture(iChannel0, uv + vec2(texelSize.x * 2.0, 0.0)).rgb * 0.04;
//    color += texture(iChannel0, uv + vec2(-texelSize.x * 2.0, 0.0)).rgb * 0.04;
//    color += texture(iChannel0, uv + vec2(0.0, texelSize.y * 2.0)).rgb * 0.04;
//    color += texture(iChannel0, uv + vec2(0.0, -texelSize.y * 2.0)).rgb * 0.04;
//    return color;
//}

//float scanlineFactor(float y)
//{
//    float pos = y * iResolution.y;
//    float s = sin(pos * 1.5 + iTime * 2.0);
//    return 1.0 - SCAN_INTENSITY * (0.5 + 0.5 * s);
//}

//vec3 colorMask(vec2 fragCoord)
//{
//    float xmod = mod(fragCoord.x + fragCoord.y * 0.5, 6.0) / 6.0;
//    vec3 mask = vec3(MASK_DARK);
//    if (xmod < 0.333) mask.r = MASK_LIGHT;
//    else if (xmod < 0.666) mask.g = MASK_LIGHT;
//    else mask.b = MASK_LIGHT;
//    return mask;
//}

//vec3 cheapGlow(vec2 uv, vec2 texelSize)
//{
//    vec3 c = vec3(0.0);
//    c += texture(iChannel0, uv + vec2(texelSize.x, 0.0)).rgb;
//    c += texture(iChannel0, uv + vec2(-texelSize.x, 0.0)).rgb;
//    c += texture(iChannel0, uv + vec2(0.0, texelSize.y)).rgb;
//    c += texture(iChannel0, uv + vec2(0.0, -texelSize.y)).rgb;
//    return c * (GLOW_STRENGTH * 0.25);
//}

//void main()
//{
//    vec2 uv = gl_FragCoord.xy / iResolution.xy;
//    vec2 texelSize = 1.0 / iResolution;

//    vec3 base = blurSample(uv, texelSize);
//    vec3 glow = cheapGlow(uv, texelSize);
//    vec3 color = mix(base, base + glow, 0.25);

//    float scan = scanlineFactor(gl_FragCoord.y / iResolution.y);
//    color *= scan;
//    color *= colorMask(gl_FragCoord.xy);

//    FragColor = vec4(color, 1.0);
//}