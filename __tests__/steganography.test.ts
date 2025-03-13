import {
  embedSecretMessage,
  extractSecretMessage,
} from '../modules/steganography';
import { User } from '../types/user';

const mockUserOne: User = {
  id: '1',
  name: 'John Doe',
  secret: '0987654321',
};

const mockUserTwo: User = {
  id: '2',
  name: 'Jane Doe',
  secret: '1234567890',
};

describe(embedSecretMessage.name, () => {
  it('should embed a secret message', () => {
    const message = embedSecretMessage({
      users: [mockUserOne, mockUserTwo],
      privateMessage: 'Hi, there how are you?',
      publicMessage: 'Hello, world!',
    });
    expect(message.length).toBe(1135);
  });
});

describe(extractSecretMessage.name, () => {
  it('should extract a secret message', () => {
    const message = embedSecretMessage({
      users: [mockUserOne, mockUserTwo],
      privateMessage: 'Hi, there how are you?',
      publicMessage: 'Hello, world!',
    });
    const extractedMessage = extractSecretMessage({
      message,
      users: [mockUserOne, mockUserTwo],
    });
    expect(extractedMessage).toBe('Hi, there how are you?');
  });
});
