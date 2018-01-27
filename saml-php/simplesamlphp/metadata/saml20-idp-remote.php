<?php
/**
 * SAML 2.0 remote IdP metadata for SimpleSAMLphp.
 *
 * Remember to remove the IdPs you don't use from this file.
 *
 * See: https://simplesamlphp.org/docs/stable/simplesamlphp-reference-idp-remote 
 */
$metadata['https://10.2.2.3/simplesaml/saml2/idp/metadata.php'] = array (
  'metadata-set' => 'saml20-idp-remote',
  'entityid' => 'https://10.2.2.3/simplesaml/saml2/idp/metadata.php',
  'SingleSignOnService' =>
  array (
    0 =>
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect',
      'Location' => 'https://10.2.2.3/simplesaml/saml2/idp/SSOService.php',
    ),
  ),
  'SingleLogoutService' =>
  array (
    0 =>
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect',
      'Location' => 'https://10.2.2.3/simplesaml/saml2/idp/SingleLogoutService.php',
    ),
  ),
  'certData' => 'MIIDhTCCAm2gAwIBAgIJAJWSkrMdkaKeMA0GCSqGSIb3DQEBC...',
  'NameIDFormat' => 'urn:oasis:names:tc:SAML:2.0:nameid-format:transient',
);

$metadata['https://sp.authpoint.cloud.watchguard.com'] = array (
  'entityid' => 'https://sp.authpoint.cloud.watchguard.com',
  'contacts' => 
  array (
  ),
  'metadata-set' => 'saml20-idp-remote',
  'SingleSignOnService' => 
  array (
    0 => 
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect',
      'Location' => 'https://staging.sp.authpoint.deu.cloud.watchguard.com/saml/aea83b6b-1323-4950-94e5-21eeb629f897/sso/spinit',
    ),
    1 => 
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST',
      'Location' => 'https://staging.sp.authpoint.deu.cloud.watchguard.com/saml/aea83b6b-1323-4950-94e5-21eeb629f897/sso/spinit',
    ),
  ),
  'SingleLogoutService' => 
  array (
    0 => 
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect',
      'Location' => 'https://staging.sp.authpoint.deu.cloud.watchguard.com/saml/aea83b6b-1323-4950-94e5-21eeb629f897/slo/spinit',
    ),
    1 => 
    array (
      'Binding' => 'urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST',
      'Location' => 'https://staging.sp.authpoint.deu.cloud.watchguard.com/saml/aea83b6b-1323-4950-94e5-21eeb629f897/slo/spinit',
    ),
  ),
  'ArtifactResolutionService' => 
  array (
  ),
  'NameIDFormats' => 
  array (
  ),
  'keys' => 
  array (
    0 => 
    array (
      'encryption' => false,
      'signing' => true,
      'type' => 'X509Certificate',
      'X509Certificate' => 'MIIFQDCCAyigAwIBAgIUdm9LpBFZIZOtZXQYIQIiY7+If0AwDQYJKoZIhvcNAQEN
BQAwWjELMAkGA1UEBhMCVVMxCzAJBgNVBAgMAldBMRAwDgYDVQQHDAdTZWF0dGxl
MRMwEQYDVQQKDApXYXRjaGd1YXJkMRcwFQYDVQQDDA53YXRjaGd1YXJkLmNvbTAe
Fw0xODAxMTgxNDI2MDJaFw0xOTAxMTgxNDI2MDJaMFoxCzAJBgNVBAYTAlVTMQsw
CQYDVQQIDAJXQTEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECgwKV2F0Y2hndWFy
ZDEXMBUGA1UEAwwOd2F0Y2hndWFyZC5jb20wggIiMA0GCSqGSIb3DQEBAQUAA4IC
DwAwggIKAoICAQDQWu2dv+oqsv3VMn4hd556Z98sH1Zlp9SFv3PYHvoyHz7PuibI
gI39zq2qJfWrzGK9PXN6WpLdQ4TMc700GzNG2MFKi0UYO2O44L0S4jj8MxH+Pn44
aoSTkYVgFMzo9n3vsvPmBEAaXB/yJObua9UNSJB4G3pet+s1DgMMCe5eA4aw7PSR
WsuubEdvzwfs96CVHiBdO49pBVb39aKriifv7jW66RAHtUA5PWWnJmy5EiCYrXpC
hdN000zBPIjoo+6BB+R7s4GTtF9dICDrtFMjgeYpk97+5Yak9MfeIFOUXucV4dnO
8EN1BEJXPupuiHB+6AxT1GiLWw4Wi/p60Pw1OVHDkpJkA+k+NKpA/z6giAlMrh6D
KIUPCm7nD3KBwvspJbQ1wA0xT9tD2oznnBYW4rKN7bNkFIrrQz7tKvQbQwKsWKLz
9dozWDedg2e842fFNMGg7XR1dDTSv1RNg8hC6q5ZyHMiiNhPijCzVczZGmNrE4r/
pRnQ7o6HtaE5jSPTJnb6O4OgbSFrVVZlDJirLeDJVYhGHgEjoMxxHOkz94Dndd2H
Y+F3DyqSZD/oVVDKH+sSTTEQNhTxlemyqPj6syb0XG1XmrN8Gp1l6Xatljic/ang
vTLm/MI6go/5Hyz/9OdeIKPNdktTbHFVRx/30u+ERo/7sOSW839cX+0TGwIDAQAB
MA0GCSqGSIb3DQEBDQUAA4ICAQA93eAbbRqjSQkeqo9ea1LgMpAlFgCuRmXKFwuD
BopxrvQgh7ussrxUA0gdnWd520E2zrMJME7geCc+zwhXWWUmBS4FCDZkZAxyK2qL
LrYFAcYnjva7bxRn4Lt0z9qsQWvHE0jzQwF0oQaEjvFyIs1d7OmlvINtsFtBYVts
SqGou64ipaGnlF2d4sOcTdlOji9JEtpsB5HAcEgpFPPxG063oWFKcFVuHpvb4um9
oqMrQAu1Xrk9uWITzRAYnIFN6TL5lBDPJTPJ+xDcfQeasGcM2ENevrlAmBXm6abQ
wSWiosCJxOIND9+kRCnE03uPvvnjgAEUnsie6yBFYCGCislgYolsjGwJ8GnqwGJy
OUIrk2ZZrDbF7s0T/R0j/mcLOxHZwc/plb5BG5Bs4jw56JDsHCOMvVkYmQNk+dtx
TvKZdeItPsohhknzwEjPrZZU/RuC8auoejR76cVQsGOKr1AILKx3Ga/D+gyzePqt
WbAAjcpOdA/DaEYEOhJ9+Zrs1u6ay5b3qZq5P48E/mMc/AMUehJA4+UAs2qcTIBq
XdWYeGzDt+z8d4CDA+lE3TlhfoOwG5m4kaQ9Ym3gy1t8yLLV0C+0oK1MIBgAXsam
rRflLHbPz6rTmdPRAghQscO8gETw+lOhr8PXEpkQjgWk+gOqRWyw1NDWZVzz+iZT
54m4YQ==
',
    ),
  ),
);
