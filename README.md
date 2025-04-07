# Steggit JS Monorepo

A monorepo containing the JavaScript implementation of Steggit, a steganography tool for hiding data within images.

## Overview

This monorepo contains multiple packages that work together to provide steganography functionality across different JavaScript environments:

- `@steggit-js/core`: Core image steganography functionality
- `@steggit-js/browser`: Browser-specific implementation
- `@steggit-js/node`: Node.js-specific implementation
- `@steggit-js/utils`: Shared utilities including encryption tools and string steganography

## Getting Started

### Prerequisites

- Node.js
- Yarn 4.7.0 or later

### Installation

```bash
# Clone the repository
git clone https://github.com/steggit/steggit-js.git
cd steggit-js

# Install dependencies
yarn install
```

## Development

### Building Packages

```bash
# Build all packages
yarn build:all

# Build specific packages
yarn build:core
yarn build:browser
yarn build:node
yarn build:utils
```

### Linting

```bash
# Lint all packages
yarn lint:all

# Lint specific packages
yarn lint:core
yarn lint:browser
yarn lint:node
yarn lint:utils
```

## Project Structure

```
steggit-js/
├── packages/
│   ├── core/      # Core steganography functionality
│   ├── browser/   # Browser-specific implementation
│   ├── node/      # Node.js-specific implementation
│   └── utils/     # Shared utilities
├── .husky/        # Git hooks
├── .vscode/       # VS Code configuration
└── .yarn/         # Yarn configuration
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`yarn commit:staged` or `yarn commit:all`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
